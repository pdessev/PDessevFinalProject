#define __GRAPHICS_INTERNAL__
#include "LCD/graphics.h"
#include "Debug/mem.h"

Color color(uint8_t r, uint8_t g, uint8_t b) {
    return (((r & 0b11111000) << 8) | ((g & 0b11111100) << 3) | (b >> 3));
}

Result init_graphics() {
    RETURN_OR_IGNORE(LTCD_Init());
    RETURN_OR_IGNORE(LTCD_Layer_Init(0));
    LCD_Clear(0, LCD_COLOR_WHITE);
    return result(0);
}

Result draw_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, Color color) {
    for (uint16_t i = x; i < x + width; i++) {
        for (uint16_t j = y; j < y + height; j++) {
            LCD_Draw_Pixel(i, j, color);
        }
    }

    return result(0);
}

Result draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, Color color) {
    // Always draw line from left to right
    if (x1 > x2){
        uint16_t temp = x1;
        x1 = x2;
        x2 = temp;
    }

    if (y1 > y2) {
        uint16_t temp = y1;
        y1 = y2;
        y2 = temp;
    }

    if ((y2 - y1) > (x2 - x1)){
        uint16_t temp1 = x1, temp2 = x2;
        x1 = y1;
        x2 = y2;
        y1 = temp1;
        y2 = temp2;

        float m = ((float)y2 - (float)y1) / ((float)x2 - (float)x1);
        float y = y1;
        for (uint16_t x = x1; x < x2; x++) {
            LCD_Draw_Pixel(y, (uint16_t) x, color);
            y += m;
        }
    } else {
        float m = ((float)y2 - (float)y1) / ((float)x2 - (float)x1);
        float y = y1;
        for (uint16_t x = x1; x < x2; x++) {
            LCD_Draw_Pixel(x, (uint16_t) y, color);
            y += m;
        }
    }
    

    return result(0);
}

Result draw_text_line(char* text, size_t len, Color color, FONT_t* font, size_t x, size_t y) {
    if (text == 0)
        return result("Null pointer provided where string was expected");

    LCD_SetFont(font);
    LCD_SetTextColor(color);

    for (size_t i = 0; i < len; i++) {
        if (text[i] == 0)
            break;

        LCD_DisplayChar(x, y, text[i]);
        x += font->Width;
    }
    return result(0);
}
