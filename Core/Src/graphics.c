#define __GRAPHICS_INTERNAL__
#include "LCD/graphics.h"
#include "Debug/mem.h"

Color* color(uint8_t red, uint8_t green, uint8_t blue) {
    Color* c = malloc(sizeof(*c));
    c->red = red;
    c->green = green;
    c->blue = blue;
    return c;
}

void free_color(Color** c) {
    free(*c);
    *c = 0;
}

uint16_t color_to_hex(Color* c) {
    return (((c->red & 0b11111000) << 8) | ((c->green & 0b11111100) << 3) | (c->blue >> 3));
}

Result init_graphics() {
    RETURN_OR_IGNORE(LTCD_Init());
    RETURN_OR_IGNORE(LTCD_Layer_Init(0));
    LCD_Clear(0, LCD_COLOR_WHITE);
    return result(0);
}

Result draw_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, Color* color) {
    uint16_t c = color_to_hex(color);

    for (uint16_t i = x; i < x + width; i++) {
        for (uint16_t j = y; j < y + height; j++) {
            LCD_Draw_Pixel(i, j, c);
        }
    }

    return result(0);
}

Result draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, Color* color) {
    uint16_t c = color_to_hex(color);
    float m = ((float)y2 - (float)y1) / ((float)x2 - (float)x1);
    for (uint16_t x = x1; x < x2; x++) {
        LCD_Draw_Pixel(x, m * x + y1, c);
    }

    return result(0);
}

Result draw_text_line(char* text, size_t len, Color* color, FONT_t* font, size_t x, size_t y) {
    if (text == 0)
        return result("Null pointer provided where string was expected");

    LCD_SetFont(font);
    LCD_SetTextColor(color_to_hex(color));

    for (size_t i = 0; i < len; i++) {
        if (text[i] == 0)
            break;

        LCD_DisplayChar(x, y, text[i]);
        x += font->Width;
    }
    return result(0);
}
