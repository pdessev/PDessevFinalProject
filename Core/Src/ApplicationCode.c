/*
 * ApplicationCode.c
 *
 *  Created on: Sep 24, 2024
 *      Author: peter
 */

#define __APPLICATION_CODE_INTERNAL__
#include "ApplicationCode.h"
#include "Debug/mem.h"
#include "Game/Game.h"
#include "LCD/graphics.h"
#include "Scheduler.h"
#include "Timer_Wrapper.h"


static struct {
    STMPE811_TouchData StaticTouchData;
    bool IsPressed;
} ScreenState; 

static EXTI_HandleTypeDef LCDTouchIRQ;

void LCDTouchScreenInterruptGPIOInit(void);

AppState* init_app() {

    Result graphics = init_graphics();
    if (is_error(&graphics)) {
        ignore(print_error(graphics));
        return NULL;
    }
    ignore(graphics);

    Result lcd = InitializeLCDTouch();
    if (is_error(&lcd)) {
        ignore(print_error(lcd));
        return NULL;
    }
    ignore(graphics);

    // This is the orientation for the board to be direclty up where the buttons
    // are vertically above the screen Top left would be low x value, high y
    // value. Bottom right would be low x value, low y value.
    ScreenState.StaticTouchData.orientation = STMPE811_Orientation_Portrait_2;
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

Result app_loop(AppState** state) {
    // AppState* s = *state;

    GameState* game = create_game(0);
    start_timer(TimerTwo);

    while (1) {
        uint32_t events = getScheduledEvents();
        for (int i = 0; i < 32; i++) {
            if (!(events & (1 << i))) {
                continue;
            }
            switch (i) {
            case EventDrawMainMenu:
                RETURN_OR_IGNORE(show_main_menu(game));
                removeSchedulerEvent(EventDrawMainMenu);
                break;
            case EventStartGame:
                // removeSchedulerEvent(EventDrawMainMenu);
                break;
            case EventDropBlock:
                removeSchedulerEvent(EventDropBlock);
                break;
            case EventMoveBlockLeft:
                removeSchedulerEvent(EventMoveBlockLeft);
                break;
            case EventMoveBlockRight:
                removeSchedulerEvent(EventMoveBlockRight);
                break;
            }
        }
    }

    free(*state);
    *state = 0;
    return result(0);
}

void print_gyro(AppState* state) {
    uint8_t id = gyro_get_id(state->g);
    int8_t temp = gyro_get_temp(state->g);

    printf("Gyro Id: %X\nTemperature: %d\n", id, temp);
}

void delay(uint32_t count) {
    char name[] = "Peter";
    char* copy = calloc(NAME_LEN, sizeof(*copy));
    for (uint32_t _ = count; _; _--) {
        for (uint8_t i = NAME_LEN; i; i--) {
            *(copy + i - 1) = *(name + i - 1);
        }
    }
    free(copy);
}

void TIM2_IRQHandler() {
    disable_IRQ(TIM2_IRQn);
    clear_timer_interrupt_flag(TimerTwo);
    addSchedulerEvent(EventDrawMainMenu);
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
    ScreenState.IsPressed = ctrlReg & 0x80;

    // Determine if it is pressed or unpressed
    if (ScreenState.IsPressed) { // Touch has been detected
        // May need to do numerous retries?
        DetermineTouchPosition(&ScreenState.StaticTouchData);
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
