#pragma once

#include <stdint.h>

#include "ErrorHandling.h"
#include "RNG/RNG.h"

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 25

#ifdef __GAME_INTERNAL__
#define BOARD_BLOCK_PIXEL_LEN 10 // approx 320 * 0.8 / 25
#include "LCD/graphics.h"
#include "Debug/mem.h"
#include "stm32f4xx_hal.h"
// #include "stdlib.h"

typedef enum BlockType {
    TypeO,
    TypeI,
    TypeS,
    TypeZ,
    TypeL,
    TypeJ,
    TypeT,
    BlockTypeLen,
} BlockType;

typedef enum BlockRotation {
    RotUp,
    RotRight,
    RotDown,
    RotLeft,
} BlockRotation;

typedef enum BlockColor {
    BlockRed,
    BlockOrange,
    BlockYellow,
    BlockGreen,
    BlockCyan,
    BlockBlue,
    BlockPurple,
    BlockColorLen,
} BlockColor;

typedef struct Point {
    int x;
    int y;
} Point;

typedef struct Block {
    BlockType type;
    BlockRotation rotation;
    BlockColor color;
    Point points[4];
    Point center;
} Block;

typedef struct LaidBlock {
    BlockColor color;
} LaidBlock;

typedef struct GameState {
    Block* active_block;
    RandomNum* rng;
    LaidBlock* board[BOARD_WIDTH][BOARD_HEIGHT];
    uint8_t main_menu_state;
} GameState;

Color* get_rgb_from_block_color(BlockColor b);

void set_block_points(Block* b);

#else
typedef void GameState;
#endif

typedef enum Direction {
    DirUp,
    DirRight,
    DirDown,
    DirLeft,
} Direction;

GameState* create_game(RandomNum* rng);

Result show_main_menu(GameState* s);

Result show_game_board(GameState* s);

void move_block(GameState* s, Direction dir);

void lay_current_block(GameState* s);

Result new_current_block(GameState* s) __attribute__((warn_unused_result));