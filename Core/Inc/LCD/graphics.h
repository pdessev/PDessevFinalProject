#pragma once

#include "LCD_Driver.h"
#include "ErrorHandling.h"
#include "stdint.h"

#ifdef __GRAPHICS_INTERNAL__
typedef struct Color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} Color;
#else
typedef void Color;
#endif
// Colors
// A color is a standard 24 bit RGB color
Color* color(uint8_t red, uint8_t green, uint8_t blue);

void free_color(Color** c);

Result init_graphics() __attribute__((warn_unused_result));

Result draw_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, Color* color) __attribute__((warn_unused_result));

Result draw_text_line(char* text, size_t len, Color* color, FONT_t* font, size_t x, size_t y) __attribute__((warn_unused_result));
