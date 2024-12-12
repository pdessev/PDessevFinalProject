#define __RNG_INTERNAL__
#include "RNG/RNG.h"
#include "InterruptControl.h"
#include "stm32f4xx_hal.h"

static RNG_HandleTypeDef rng;

Result init_random() {
    rng.Instance = RNG;
    __RNG_CLK_ENABLE();
    HAL_StatusTypeDef res = HAL_RNG_Init(&rng);
    if (res != HAL_OK) {
        char str[] = "Error initializing random number: 0";
        str[32] = res + '0';
        return result(str);
    }
    enable_IRQ(HASH_RNG_IRQn);
    return result(0);
}

Result generate_random(uint32_t* dest) {
    // HAL_StatusTypeDef res = HAL_RNG_GenerateRandomNumber_IT(&rng);
    HAL_StatusTypeDef res = HAL_RNG_GenerateRandomNumber(&rng, dest);
    if (res == HAL_ERROR) {
        return result("Error generating random number");
    }
    return result(0);
}

uint32_t get_random() { return HAL_RNG_ReadLastRandomNumber(&rng); }