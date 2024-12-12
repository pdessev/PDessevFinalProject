#pragma once

#include <stdint.h>
#include "ErrorHandling.h"

Result init_random();

Result generate_random(uint32_t* dest);

// uint32_t get_random();