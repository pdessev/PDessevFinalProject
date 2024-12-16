/**
 * @file            RNG.h
 * @brief           Header for `RNG.c` \n
 *                    This file contains functions and structure 
 *                    definitions for the random number generator interface.
 * \n Created on: Sep 24, 2024
 * \n Author: peter
 * 
 */
#pragma once

#include <stdint.h>
#include "ErrorHandling.h"

/** 
 * Initilaizes the random number generator.
 * # Inputs
 * None
 * # Returns
 * - `Reuslt(char*)` on error.
 * - `Result(0)` on successful initialization.
 */
Result init_random();

/** 
 * Generates a random number.
 * # Inputs
 * - `uint32_t* dest`: The location to store the newly generated random number.
 * # Returns
 * - `Reuslt(char*)` on error.
 * - `Result(0)` on successful generation.
 */
Result generate_random(uint32_t* dest);

// uint32_t get_random();