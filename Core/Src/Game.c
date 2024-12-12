#define __GAME_INTERNAL__
#include "Game/Game.h"

#include <stdint.h>
#include <stdio.h>

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

Result show_game_board(GameState* s, uint32_t time) {
    const static uint16_t board_left = (LCD_PIXEL_WIDTH / 2) - ((BOARD_WIDTH * BOARD_BLOCK_PIXEL_LEN) / 2);
    const static uint16_t board_right = LCD_PIXEL_WIDTH - board_left;
    const static uint16_t board_up = (LCD_PIXEL_HEIGHT / 2) - ((BOARD_HEIGHT * BOARD_BLOCK_PIXEL_LEN) / 2);
    const static uint16_t board_down = LCD_PIXEL_HEIGHT - board_up;

    LCD_Clear(0, color(255, 255, 255));

    // Draw set blocks
    for (uint8_t i = 0; i < BOARD_WIDTH; i++) {
        for (uint8_t j = 0; j < BOARD_HEIGHT; j++) {
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

    // Draw border
    Color line_color = color(0, 0, 0);
    RETURN_OR_IGNORE(draw_line(board_left, board_up, board_right, board_up, line_color));     // Top
    RETURN_OR_IGNORE(draw_line(board_left, board_down, board_right, board_down, line_color)); // Bottom
    RETURN_OR_IGNORE(draw_line(board_left, board_up, board_left, board_down, line_color));    // Left
    RETURN_OR_IGNORE(draw_line(board_right, board_up, board_right, board_down, line_color));  // Right

    // Draw time
    char time_string[13] = { 0 };
    snprintf(time_string, 12, "%ld:%ld", time / 60, time % 60);
    RETURN_OR_IGNORE(draw_text_line(time_string, 6, color(0, 0, 0), &Font12x12, 5, 5));

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

    // Generate a new block which is rotated clockwise.
    Block* block =
        generate_new_block(s->active_block->type, s->active_block->color, s->active_block->rotation,
                           point(s->active_block->center.x, s->active_block->center.y));
    block->center.x += delta_x;
    block->center.y += delta_y;
    set_block_points(block);

    // It is possible that the block is now outside of the board.
    // Correct this.
    for (uint8_t i = 0; i < 4; i++) {
        int x_coord = block->points[i].x;
        int y_coord = block->points[i].y;

        // X Correction
        if (x_coord < 0 || x_coord >= BOARD_WIDTH || y_coord < 0 ||
            y_coord >= BOARD_HEIGHT) { // Out of bounds
            destroy_block(&block);
            return result("Unable to move block, out of bounds detected.");
        }

        if (s->board[x_coord][y_coord]) {
            destroy_block(&block);
            return result("Unable to move block, collision detected.");
        }
    }
    destroy_block(&s->active_block);
    s->active_block = block;
    return result(0);
}

Result rotate_block(GameState* s) {
    // Generate a new block which is rotated clockwise.
    Block* block =
        generate_new_block(s->active_block->type, s->active_block->color, s->active_block->rotation,
                           point(s->active_block->center.x, s->active_block->center.y));
    block->rotation += 1;
    block->rotation %= 4;
    set_block_points(block);

    // It is possible that the block is now outside of the board.
    // Correct this.
    for (uint8_t i = 0; i < 4; i++) {
        int x_coord = block->points[i].x;
        int y_coord = block->points[i].y;

        // X Correction
        int x_error = 0;
        if (x_coord < 0) { // Shift all to the right
            x_error = x_coord;
        } else if (x_coord > BOARD_WIDTH) { // Shift all to the left
            x_error = x_coord - BOARD_WIDTH;
        }
        block->center.x -= x_error;

        // Y Correction
        int y_error = 0;
        if (y_coord < 0) { // Shift all to the down
            y_error = y_coord;
        } else if (y_coord > BOARD_HEIGHT) { // Shift all to the up
            y_error = y_coord - BOARD_HEIGHT;
        }
        block->center.y -= y_error;

        // Check if any collisions are present. If there are, then don't rotate the block
        if (s->board[x_coord - x_error][y_coord - y_error]) {
            destroy_block(&block);
            return result("Unable to rotate block due to collisions");
        }
    }
    set_block_points(block);

    destroy_block(&s->active_block);
    s->active_block = block;
    return result(0);
}

Result lay_current_block(GameState* s) {
    for (size_t i = 0; i < 4; i++) {
        Block* b = s->active_block;
        int x = b->points[i].x;
        int y = b->points[i].y;

        if (x < 0 || x >= BOARD_WIDTH || y < 0 || y > BOARD_HEIGHT) {
            destroy_block(&s->active_block);
            return result("Error laying block, out of bounds indecies");
        }

        if (s->board[x][y]) {
            destroy_block(&s->active_block);
            return result("Error laying block, unexpected collision");
        }

        s->board[x][y] = calloc(1, sizeof(*s->board[x][y]));
        s->board[b->points[i].x][b->points[i].y]->color = b->color;
    }
    destroy_block(&s->active_block);
    return result(0);
}

Result new_current_block(GameState* s, BlockType type, BlockColor color) {
    if (s->active_block) {
        return result("Unable to generate new block, block already exists");
    }
    s->active_block = generate_new_block(type, color, RotUp, point(BOARD_WIDTH / 2, 0));
    // It is possible that the block is now outside of the board.
    // Correct this.
    for (uint8_t i = 0; i < 4; i++) {
        int x_coord = s->active_block->points[i].x;
        int y_coord = s->active_block->points[i].y;

        // X Correction
        int x_error = 0;
        if (x_coord < 0) { // Shift all to the right
            x_error = x_coord;
        } else if (x_coord >= BOARD_WIDTH) { // Shift all to the left
            x_error = x_coord - BOARD_WIDTH;
        }
        s->active_block->center.x -= x_error;

        // Y Correction
        int y_error = 0;
        if (y_coord < 0) { // Shift all to the down
            y_error = y_coord;
        } else if (y_coord >= BOARD_HEIGHT) { // Shift all to the up
            y_error = y_coord - BOARD_HEIGHT;
        }
        s->active_block->center.y -= y_error;

        // Check if any collisions are present. If there are, then don't rotate the block
        // if (s->board[x_coord - x_error][y_coord - y_error]) {
        //     destroy_block(&block);
        //     return result("Unable to rotate block due to collisions");
        // }
    }
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
