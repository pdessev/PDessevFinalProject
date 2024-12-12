#pragma once

#include "LCD_Driver.h"
#include "ErrorHandling.h"
#include "stdint.h"

#ifdef __GRAPHICS_INTERNAL__
#else
#endif
typedef uint16_t Color;

Color color(uint8_t r, uint8_t g, uint8_t b);

Result init_graphics() __attribute__((warn_unused_result));

Result draw_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, Color color) __attribute__((warn_unused_result));

Result draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, Color color);

Result draw_text_line(char* text, size_t len, Color color, FONT_t* font, size_t x, size_t y) __attribute__((warn_unused_result));
