/*
 * ErrorHandling.c
 *
 *  Created on: Nov 12, 2024
 *      Author: peter
 */
// #include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define __ERROR_HANDLING_INTERNAL__
#include "ErrorHandling.h"
#include "Debug/mem.h"

size_t strnlen_s(const char* str, size_t strsz){
    if (str == 0)
        return 0;
    return strnlen(str, strsz);
}

Result result(char* str) {
    size_t len = strnlen_s(str, MAX_ERROR_LENGTH);
    if (len == 0) {
        return calloc(1, sizeof(__Result));
    }
    Result res = malloc(sizeof(*res));
    res->msg = malloc(sizeof(*(res->msg)) * (len + 1));
    strncpy(res->msg, str, len);
    res->error = 1;
    return res;
}

Result print_error(Result e) {
    if(is_error(&e) && printf(e->msg) < 0){
        return result("Output or encoding error");
    } else {
        return result(0);
    }
}

void ignore(Result e) {
    if (is_error(&e))
        free(e->msg);
    
    free(e);
}

bool is_error(Result* e) {
    return (*e)->error;
}

bool c_is_error(Result e) {
    bool b = is_error(&e);
    ignore(e);
    return b;
}

void a_assert(bool check){
    while (!check);
}

void e_assert(Result e) {
    while (is_error(&e));
}