#define __GAME_INTERNAL__
#include "Game/Game.h"

#include <stdint.h>

Point point(int x, int y) {
    Point p = { .x = x, .y = y };
    return p;
}

Block* generate_new_block(BlockType type, BlockColor color, BlockRotation rot, Point pos) {
    Block* block = calloc(1, sizeof(*block));
    block->type = type;
    block->rotation = rot;
    block->color = color;
    block->center.x = pos.x;
    block->center.y = pos.y;
    set_block_points(block);
    return block;
}

void destroy_block(Block** b) {
    free(*b);
    *b = 0;
}

GameState* create_game() {
    GameState* g = calloc(1, sizeof(*g));
    g->main_menu_state = 0;
    return g;
}

void free_game(GameState** g) {
    free((*g)->active_block);
    for (uint16_t i = 0; i < BOARD_WIDTH; i++) {
        for (uint16_t j = 0; j < BOARD_HEIGHT; j++) {
            if (!(*g)->board[i][j]) {
                continue;
            }
            free((*g)->board[i][j]);
        }
    }
    free(*g);
    *g = 0;
}

Result show_main_menu(GameState* s) {
    const static uint16_t block_positions[][2] = { { 38, 190 }, { 96, 190 },  { 144, 190 }, { 192, 190 },
                                                   { 60, 240 }, { 120, 240 }, { 180, 240 } };

    Block* blocks[BlockTypeLen];
    for (uint8_t i = 0; i < BlockTypeLen; i++) {
        blocks[i] = generate_new_block(i, i, s->main_menu_state, point(0, 0));
    }

    // Text "TETRIS"
    LCD_Clear(0, color(255, 255, 255));
    RETURN_OR_IGNORE_CLOSURE(draw_text_line("TETRIS", 6, color(0, 0, 0), &Font16x24,
                                            (LCD_PIXEL_WIDTH / 2) - 3 * Font16x24.Width,
                                            (LCD_PIXEL_HEIGHT / 3)),
                             {
                                 for (int __free_blocks = 0; __free_blocks < BlockTypeLen; __free_blocks++) {
                                     destroy_block(&blocks[__free_blocks]);
                                 }
                             });

    // Loop through blocks
    for (uint8_t i = 0; i < BlockTypeLen; i++) {
        Color c = get_rgb_from_block_color(i); // Get Color
        blocks[i]->rotation = s->main_menu_state;
        set_block_points(blocks[i]);

        for (uint8_t k = 0; k < 4; k++) {
            uint16_t x = block_positions[i][0] + BOARD_BLOCK_PIXEL_LEN * blocks[i]->points[k].x;
            uint16_t y = block_positions[i][1] + BOARD_BLOCK_PIXEL_LEN * blocks[i]->points[k].y;

            RETURN_OR_IGNORE_CLOSURE(draw_rect(x, y, BOARD_BLOCK_PIXEL_LEN, BOARD_BLOCK_PIXEL_LEN, c), {
                for (int __free_blocks = 0; __free_blocks < BlockTypeLen; __free_blocks++) {
                    destroy_block(&blocks[__free_blocks]);
                }
            });
        }
    }

    for (uint8_t i = 0; i < BlockTypeLen; i++) {
        free(blocks[i]);
    }

    s->main_menu_state += 1;
    s->main_menu_state %= 4;
    return result(0);
}

Result show_game_board(GameState* s) {
    const static uint16_t board_left = (BOARD_WIDTH * BOARD_BLOCK_PIXEL_LEN) / 2;
    const static uint16_t board_right = LCD_PIXEL_WIDTH - board_left;
    const static uint16_t board_up = (BOARD_HEIGHT * BOARD_BLOCK_PIXEL_LEN) / 2;
    const static uint16_t board_down = LCD_PIXEL_HEIGHT - board_up;

    LCD_Clear(0, color(255, 255, 255));
    // Draw Border
    Color line_color = color(0, 0, 0);
    RETURN_OR_IGNORE(
        draw_line(board_left - 1, board_up - 1, board_right + 1, board_up - 1, line_color)); // Top
    RETURN_OR_IGNORE(
        draw_line(board_left - 1, board_down + 1, board_right + 1, board_down + 1, line_color)); // Bottom
    RETURN_OR_IGNORE(
        draw_line(board_left - 1, board_up - 1, board_left - 1, board_down + 1, line_color)); // Left
    RETURN_OR_IGNORE(
        draw_line(board_right + 1, board_up - 1, board_right + 1, board_down + 1, line_color)); // Right

    // Draw set blocks
    for (uint8_t i = 0; i < BOARD_WIDTH; i++) {
        for (uint8_t j = 0; j < BOARD_HEIGHT; i++) {
            if (s->board[i][j]) {
                RETURN_OR_IGNORE(draw_rect(board_left + i * BOARD_BLOCK_PIXEL_LEN,
                                           board_up + j * BOARD_BLOCK_PIXEL_LEN, BOARD_BLOCK_PIXEL_LEN,
                                           BOARD_BLOCK_PIXEL_LEN,
                                           get_rgb_from_block_color(s->board[i][j]->color)));
            }
        }
    }

    // Draw active block
    for (uint8_t i = 0; i < 4; i++) {
        Block* b = s->active_block;
        RETURN_OR_IGNORE(draw_rect(board_left + b->points[i].x * BOARD_BLOCK_PIXEL_LEN,
                                   board_up + b->points[i].y * BOARD_BLOCK_PIXEL_LEN, BOARD_BLOCK_PIXEL_LEN,
                                   BOARD_BLOCK_PIXEL_LEN, get_rgb_from_block_color(b->color)));
    }

    return result(0);
}

Result move_block(GameState* s, Direction dir) {
    int delta_x = 0, delta_y = 0;
    switch (dir) {
    case DirUp:
        delta_y = -1;
        break;
    case DirRight:
        delta_x = 1;
        break;
    case DirDown:
        delta_y = 1;
        break;
    case DirLeft:
        delta_x = -1;
        break;
    }

    for (uint8_t i = 0; i < 4; i++) {
        Point* cur = &s->active_block->points[i];

        if (((int16_t)cur->x + (int16_t)delta_x < 0) || ((int16_t)cur->y + (int16_t)delta_y < 0) ||
            ((int16_t)cur->x + (int16_t)delta_x > BOARD_WIDTH - 1) ||
            ((int16_t)cur->y + (int16_t)delta_y > BOARD_HEIGHT - 1)) {
            return result("Unable to move block, out of bounds detected.");
        }

        if (s->board[cur->x + delta_x][cur->y + delta_y]) {
            return result("Unable to move block, collision detected.");
        }
    }

    for (uint8_t i = 0; i < 4; i++) {
        s->active_block->points[i].x += delta_x;
        s->active_block->points[i].y += delta_y;
    }
    return result(0);
}

void lay_current_block(GameState* s) {
    for (size_t i = 0; i < 4; i++) {
        Block* b = s->active_block;
        // s->board[b->points[i].x][b->points[i].y]->valid = true;
        s->board[b->points[i].x][b->points[i].y]->color = b->color;
    }
    destroy_block(&s->active_block);
}

Result new_current_block(GameState* s, BlockType type, BlockColor color) {
    if (s->active_block) {
        return result("Unable to generate new block, block already exists");
    }
    s->active_block = generate_new_block(type, color, RotUp, point(BOARD_WIDTH / 2, 4));
    return result(0);
}

Color get_rgb_from_block_color(BlockColor b) {
    switch (b) {
    case BlockRed:
        return color(255, 0, 0);
    case BlockOrange:
        return color(255, 165, 0);
    case BlockYellow:
        return color(255, 245, 0);
    case BlockGreen:
        return color(0, 255, 0);
    case BlockCyan:
        return color(0, 255, 255);
    case BlockBlue:
        return color(0, 0, 255);
    case BlockPurple:
        return color(238, 130, 238);
    case BlockColorLen:
        return color(0, 0, 0);
    }
    return color(127, 127, 127);
}

void set_block_points(Block* b) {
    const static int8_t type_rot_point_dim_deltas[][4][4][2] = {
        // O type
        {
            { { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 } }, // Up
            { { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 } }, // Right
            { { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 } }, // Down
            { { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 } }  // Left
        },
        // I type
        {
            { { -1, 1 }, { -1, 0 }, { -1, -1 }, { -1, -2 } }, // Up
            { { -2, -1 }, { -1, -1 }, { 0, -1 }, { 1, -1 } }, // Right
            { { 0, -2 }, { 0, -1 }, { 0, 0 }, { 0, 1 } },     // Down
            { { 1, 0 }, { 0, 0 }, { -1, 0 }, { -2, 0 } }      // Left
        },
        // S type
        {
            { { 0, 1 }, { 0, 0 }, { -1, 0 }, { -1, -1 } }, // Up
            { { -1, 0 }, { 0, 0 }, { 0, -1 }, { 1, -1 } }, // Right
            { { 0, -1 }, { 0, 0 }, { 1, 0 }, { 1, 1 } },   // Down
            { { 1, 0 }, { 0, 0 }, { 0, 1 }, { -1, 1 } }    // Left
        },
        // Z type
        {
            { { -1, 1 }, { -1, 0 }, { 0, 0 }, { 0, -1 } }, // Up
            { { -1, -1 }, { 0, -1 }, { 0, 0 }, { 1, 0 } }, // Right
            { { 1, -1 }, { 1, 0 }, { 0, 0 }, { 0, 1 } },   // Down
            { { 1, 1 }, { 0, 1 }, { 0, 0 }, { -1, 0 } }    // Left
        },
        // L type
        {
            { { 1, 1 }, { 0, 1 }, { 0, 0 }, { 0, -1 } },   // Up
            { { -1, 1 }, { -1, 0 }, { 0, 0 }, { 1, 0 } },  // Right
            { { -1, -1 }, { 0, -1 }, { 0, 0 }, { 0, 1 } }, // Down
            { { 1, -1 }, { 1, 0 }, { 0, 0 }, { -1, 0 } }   // Left
        },
        // J type
        {
            { { -1, 1 }, { 0, 1 }, { 0, 0 }, { 0, -1 } },  // Up
            { { -1, -1 }, { -1, 0 }, { 0, 0 }, { 1, 0 } }, // Right
            { { 1, -1 }, { 0, -1 }, { 0, 0 }, { 0, 1 } },  // Down
            { { 1, 1 }, { 1, 0 }, { 0, 0 }, { -1, 0 } }    // Left
        },
        // T type
        {
            { { 1, 1 }, { 0, 1 }, { -1, 1 }, { 0, 0 } },    // Up
            { { -1, 1 }, { -1, 0 }, { -1, -1 }, { 0, 0 } }, // Right
            { { -1, -1 }, { 0, -1 }, { 1, -1 }, { 0, 0 } }, // Down
            { { 1, -1 }, { 1, 0 }, { 1, 1 }, { 0, 0 } }     // Left
        }

    };
    for (int i = 0; i < 4; i++) {
        int8_t delta_x = type_rot_point_dim_deltas[b->type][b->rotation][i][0];
        int8_t delta_y = type_rot_point_dim_deltas[b->type][b->rotation][i][1];

        b->points[i].x = b->center.x + delta_x;
        b->points[i].y = b->center.y + delta_y;
    }
}

void correct_block_position(Point* p) {
    for (uint8_t i = 0; i < 4; i++) {
        // X Correction
        if (p[i].x < 0) { // Shift all to the right
            for (uint8_t j = 0; i < 4; i++) {
                p[j].x += 1;
            }
        } else if (p[i].x >= BOARD_WIDTH) { // Shift all to the left
            for (uint8_t j = 0; i < 4; i++) {
                p[j].x -= 1;
            }
        }

        // Y Correction
        if (p[i].y < 0) { // Shift all to the down
            for (uint8_t j = 0; i < 4; i++) {
                p[j].y += 1;
            }
        } else if (p[i].y >= BOARD_HEIGHT) { // Shift all to the up
            for (uint8_t j = 0; i < 4; i++) {
                p[j].y -= 1;
            }
        }
    }
}