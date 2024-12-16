/**
 * @file            ErrorHandling.h
 * @note            Any `Result` returned from any function MUST be used by a 
 *                  function in this file to avoid leaking memory. 
 * @brief           Header for `ErrorHandling.c` \n
 *                    This file contains functions and structure 
 *                    definitions for the error handling interface.
 * \n Created on: Sep 24, 2024
 * \n Author: peter
 * 
 */
#pragma once
#include <stdbool.h>

#define MAX_ERROR_LENGTH 500

#ifdef __ERROR_HANDLING_INTERNAL__
/** 
 * Result object. Wrapped by `Result` for safety.
 * 
 */
typedef struct __Result {
    bool error;
    char* msg;
} __Result;

typedef __Result* Result;
#else
typedef void* Result;
#endif

/** 
 * Returns the current error from the function this macro is called from, 
 * or ignores it if not an error. Safe for any input, function calls included.
 */
#define RETURN_OR_IGNORE(__error) { Result __e_ROI_int = __error; if (is_error(&__e_ROI_int)) { return __e_ROI_int; } ignore(__e_ROI_int); }

/** Returns the current error from the function this macro is called from, or ignores it if not an error.
 * Safe for any input, function calls included. `closure` is a code block to be executed before returning
 * if an error is found. `closure` is not exectued on pass. 
 */
#define RETURN_OR_IGNORE_CLOSURE(__error, __closure__) { Result __e_ROI_int = __error; if (is_error(&__e_ROI_int)) { { __closure__ } return __e_ROI_int; } ignore(__e_ROI_int); }

/** 
 * Produces a `Result` with error message `str`. 
 * # Inputs
 * - `char* str`: Error message. Input `0` for no error. 
 * # Returns
 * - `Result`: The `Result` object.
 */
Result result(char* str) __attribute__((warn_unused_result));

/** 
 * Prints the error message of an error `Result`. Does nothing on non-error. Consumes the input `Result`.
 * # Inputs
 * - `Result e`: `Result` to be printed. 
 * # Returns
 * Void
 */
void print_error(Result e);

/** 
 * Consumes the input `Result`, doing nothing.
 * # Inputs
 * - `Result e`: `Result` to be consumed. 
 * # Returns
 * Void
 */
void ignore(Result e);

/** 
 * Returns true if the input `Result*` is an error.
 * # Note
 * Does not consume the error.
 * # Inputs
 * - `Result* e`: Error to be checked. 
 * # Returns
 * - `true` if the inptut is an error.
 * - `false` otherwise.
 * 
 */
bool is_error(Result* e);

/** 
 * Returns true if the input `Result` is an error, consuming the error.
 * # Inputs
 * - `Result e`: Error to be checked. 
 * # Returns
 * - `true` if the inptut is an error.
 * - `false` otherwise.
 * 
 */
bool c_is_error(Result e);

/** 
 * Asserts that the input value is true.
 * # Inputs
 * - `bool check`: input to be asserted. 
 * # Returns
 * - `void` if the inptut is true.
 * - Does not retrun otherwise.
 * 
 */
void a_assert(bool check);

/** 
 * Asserts that the input value is not an error.
 * # Inputs
 * - `Result e`: Error to be checked. 
 * # Returns
 * - `void` if the inptut is true.
 * - Does not retrun otherwise.
 * 
 */
void e_assert(Result e);