/*
 * ApplicationCode.c
 *
 *  Created on: Sep 24, 2024
 *      Author: peter
 */

#define __APPLICATION_CODE_INTERNAL__
#include "ApplicationCode.h"
#include "Debug/mem.h"
#include "LCD/graphics.h"
#include "Scheduler.h"
#include "Timer_Wrapper.h"
#include "LCD/LCD_Driver.h"
#include "stm32f4xx_hal.h"
#include "Button_Driver.h"
#include "Scheduler.h"


static struct {
    STMPE811_TouchData StaticTouchData;
    bool IsPressed;
} screen_state;

// static uint32_t rng_result;

static bool timer_is_triggered;

static EXTI_HandleTypeDef LCDTouchIRQ;

void LCDTouchScreenInterruptGPIOInit(void);

Result main_menue_event(AppState* a);
Result game_event(AppState* a);
void update_app_state(AppState* a);

AppState* init_app() {
    // ----
    // LCD
    // ----
    Result graphics = init_graphics();
    if (is_error(&graphics)) {
        // ignore(print_error(graphics));
        print_error(graphics);
        return NULL;
    }
    ignore(graphics);

    // ----
    // LCD
    // ----
    Result lcd = InitializeLCDTouch();
    if (is_error(&lcd)) {
        // ignore(print_error(lcd));
        print_error(lcd);
        return NULL;
    }
    ignore(graphics);

    // This is the orientation for the board to be direclty up where the buttons
    // are vertically above the screen Top left would be low x value, high y
    // value. Bottom right would be low x value, low y value.
    screen_state.StaticTouchData.orientation = STMPE811_Orientation_Portrait_2;
    LCDTouchScreenInterruptGPIOInit();

    // ----
    // Gyro
    // ----
    // state->g = gyro_init();
    // HAL_Delay(1000);
    // gyro_power(state->g);
    // gyro_config_regs(state->g);

    // ----
    // Button
    // ----
    init_interrupt_button(UserButton);

    // ----
    // RNG
    // ----
    Result rng = init_random();
    if (is_error(&rng)) {
        // ignore(print_error(rng));
        print_error(rng);
        return NULL;
    }
    ignore(rng);

    // rng = generate_random();
    // if (is_error(&rng)) {
    //     ignore(print_error(rng));
    //     return NULL;
    // }
    // ignore(rng);

    // ----
    // Timers
    // ----
    GpTimerConfig* cfg = calloc(1, sizeof(*cfg));
    cfg->Prescaler = timer_calculate_prescalar(1);
    cfg->AutoReload = timer_calculate_AutoReload(1, cfg->Prescaler);
    cfg->ClockDivision = ClockDivisionOne;
    cfg->CenterAlignMode = EdgeAlign;
    cfg->Direction = CountUp;
    cfg->AutoReloadBuffer = EnableAutoReloadBuffer;
    init_timer(TimerTwo, cfg);
    free(cfg);

    // ----
    // Events
    // ----
    addSchedulerEvent(EventDrawMainMenu);

    AppState* state = calloc(1, sizeof(*state));

    return state;
}

void free_app(AppState** state) {
    if ((*state)->Game) {
        free_game(&(*state)->Game);
    }
    if ((*state)->Gyro) {
        free_gyro(&(*state)->Gyro);
    }

    free(*state);
    *state = 0;
}

Result app_loop(AppState** state) {
    AppState* a = *state;

    a->Game = create_game(0);
    start_timer(TimerTwo);

    while (1) {
        RETURN_OR_IGNORE_CLOSURE(generate_random(&a->Random), {free_app(state);});
        // printf("%ld\n", a->Random);
        update_app_state(a);

        uint32_t events = getScheduledEvents();
        for (int i = 0; i < 32; i++) {
            if (!(events & (1 << i))) {
                continue;
            }
            switch (i) {
            case EventDrawMainMenu:
                RETURN_OR_IGNORE_CLOSURE(main_menue_event(a), {free_app(state);});
                break;
            case EventStartGame:
                RETURN_OR_IGNORE_CLOSURE(game_event(a), {free_app(state);});
                removeSchedulerEvent(EventStartGame);
                break;
            }
        }
        // printf("Random: %ld\n", get_random());
//        HAL_Delay(500);
    }
    
    free_app(state);
    return result(0);
}

Result main_menue_event(AppState* a) {
    if (a->ScreenTouched) {
        stop_timer(TimerTwo);
        addSchedulerEvent(EventStartGame);
        removeSchedulerEvent(EventDrawMainMenu);
    }
    if (a->TimerEvent) {
        return show_main_menu(a->Game);
    }
    return result(0);
}

Result game_event(AppState* a) {
    BlockType t = a->Random % BlockTypeLen;
    a->Random /= BlockTypeLen;
    BlockColor c = a->Random % BlockColorLen;
    a->Random /= BlockColorLen;
    // Error means that there is still a block in play
    if(c_is_error(new_current_block(a->Game, t, c))) {
        if(a->TimerEvent){
            Result move_res = move_block(a->Game, DirDown);
            if (is_error(move_res)) {
                lay_current_block(a->Game);
            }
        }
    }
    return show_game_board(a->Game);
}

void update_app_state(AppState* a) {
    a->ScreenTouched = screen_state.IsPressed;
    a->ScreenTouchedPos.x = screen_state.StaticTouchData.x;
    a->ScreenTouchedPos.y = screen_state.StaticTouchData.y;
    a->TimerEvent = timer_is_triggered;
    // a->Random = rng_result; 

    timer_is_triggered = false;
}

void TIM2_IRQHandler() {
    disable_IRQ(TIM2_IRQn);
    clear_timer_interrupt_flag(TimerTwo);
    // addSchedulerEvent(EventDrawMainMenu);
    timer_is_triggered = true;
    clear_pending_IRQ(TIM2_IRQn);
    enable_IRQ(TIM2_IRQn);
}

void EXTI0_IRQHandler() {
    disable_IRQ(EXTI0_IRQn);
    __HAL_GPIO_EXTI_CLEAR_IT(EXTI_LINE_1);

    clear_pending_IRQ(EXTI0_IRQn);
    enable_IRQ(EXTI0_IRQn);
}

// TouchScreen Interrupt
void LCDTouchScreenInterruptGPIOInit(void) {
    GPIO_InitTypeDef LCDConfig = { 0 };
    LCDConfig.Pin = GPIO_PIN_15;
    LCDConfig.Mode = GPIO_MODE_IT_RISING_FALLING;
    LCDConfig.Pull = GPIO_NOPULL;
    LCDConfig.Speed = GPIO_SPEED_FREQ_HIGH;

    // Clock enable
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // GPIO Init
    HAL_GPIO_Init(GPIOA, &LCDConfig);

    // Interrupt Configuration
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

    LCDTouchIRQ.Line = EXTI_LINE_15;
}

#define TOUCH_DETECTED_IRQ_STATUS_BIT (1 << 0) // Touchscreen detected bitmask

static uint8_t statusFlag;

void EXTI15_10_IRQHandler() {
    HAL_NVIC_DisableIRQ(EXTI15_10_IRQn); // May consider making this a
                                         // universial interrupt guard
                                         
    while (!STMPE811_Read(STMPE811_FIFO_SIZE));

    // Disable touch interrupt bit on the STMPE811
    uint8_t currentIRQEnables = ReadRegisterFromTouchModule(STMPE811_INT_EN);
    WriteDataToTouchModule(STMPE811_INT_EN, 0x00);

    // Clear the interrupt bit in the STMPE811
    statusFlag = ReadRegisterFromTouchModule(STMPE811_INT_STA);
    uint8_t clearIRQData =
        (statusFlag | TOUCH_DETECTED_IRQ_STATUS_BIT); // Write one to clear bit
    WriteDataToTouchModule(STMPE811_INT_STA, clearIRQData);

    uint8_t ctrlReg = ReadRegisterFromTouchModule(STMPE811_TSC_CTRL);
    screen_state.IsPressed = ctrlReg & 0x80;

    // Determine if it is pressed or unpressed
    if (screen_state.IsPressed) { // Touch has been detected
        // May need to do numerous retries?
        DetermineTouchPosition(&screen_state.StaticTouchData);
    }

    STMPE811_Write(STMPE811_FIFO_STA, 0x01);
    STMPE811_Write(STMPE811_FIFO_STA, 0x00);

    // Re-enable IRQs
    WriteDataToTouchModule(STMPE811_INT_EN, currentIRQEnables);
    HAL_EXTI_ClearPending(&LCDTouchIRQ, EXTI_TRIGGER_RISING_FALLING);

    HAL_NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

    // Potential ERRATA? Clearing IRQ bit again due to an IRQ being triggered
    // DURING the handling of this IRQ..
    WriteDataToTouchModule(STMPE811_INT_STA, clearIRQData);
}
