/*
 * ErrorHandling.c
 *
 *  Created on: Nov 12, 2024
 *      Author: peter
 */

#include "ErrorHandling.h"

void a_assert(bool check){
    while (!check);
}
