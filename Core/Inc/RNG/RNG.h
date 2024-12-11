#pragma once

#include <stdint.h>

#ifdef __RNG_INTERNAL__
typedef struct RandomNum {
    
} RandomNum;
#else
typedef void RandomNum;
#endif

RandomNum* new_random_num();

uint8_t free_random_num(RandomNum** r);

uint8_t init_random_num(RandomNum* r);

uint32_t get_random_num(RandomNum* r);