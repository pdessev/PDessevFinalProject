/*
 * ErrorHandling.c
 *
 *  Created on: Nov 12, 2024
 *      Author: peter
 */
// #include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>

#define __ERROR_HANDLING_INTERNAL__
#include "Debug/mem.h"
#include "ErrorHandling.h"

size_t strnlen_s(const char* str, size_t strsz) {
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
    str[len] = '\0';
    res->error = 1;
    return res;
}

void print_error(Result e) {
    if (is_error(&e) && printf("%s\n", e->msg) < 0);
    ignore(e);
}

void ignore(Result e) {
    if (is_error(&e))
        free(e->msg);

    free(e);
}

bool is_error(Result* e) { return (*e)->error; }

bool c_is_error(Result e) {
    bool b = is_error(&e);
    ignore(e);
    return b;
}

void a_assert(bool check) {
    while (!check)
        ;
}

void e_assert(Result e) {
    while (is_error(&e))
        ;
}