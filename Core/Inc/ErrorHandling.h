/*
 * ErrorHandling.h
 *
 *  Created on: Nov 12, 2024
 *      Author: peter
 */

#pragma once
#include <stdbool.h>

#define MAX_ERROR_LENGTH 500

#ifdef __ERROR_HANDLING_INTERNAL__
typedef struct __Result {
    bool error;
    char* msg;
} __Result;

typedef __Result* Result;
#else
typedef void* Result;
#endif

// Returns the current error from the function this macro is called from, or ignores it if not an error
// Safe for any input, function calls included
#define RETURN_OR_IGNORE(__error) { Result __e_ROI_int = __error; if (is_error(&__e_ROI_int)) { return __e_ROI_int; } ignore(__e_ROI_int); }

// Returns the current error from the function this macro is called from, or ignores it if not an error
// Safe for any input, function calls included. Closure is a code block to be executed before returning
// if an error is found. closure is not exectued on pass. 
#define RETURN_OR_IGNORE_CLOSURE(__error, __closure__) { Result __e_ROI_int = __error; if (is_error(&__e_ROI_int)) { { __closure__ } return __e_ROI_int; } ignore(__e_ROI_int); }

Result result(char* str) __attribute__((warn_unused_result));

void print_error(Result e);

void ignore(Result e);

bool is_error(Result* e);

bool c_is_error(Result e);

void a_assert(bool check);

void e_assert(Result e);