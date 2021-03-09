#pragma once
#include "ConsoleScreen.h"
#include <stdbool.h>

#define FOOD   -1
#define BORDER -2
#define EMPTY  0

#define GAME_HEIGHT 30
#define GAME_WIDTH  GAME_HEIGHT
#define GAME_AREA   (GAME_HEIGHT * GAME_WIDTH)

// #define ACTUAL_HEIGHT 22
#define HEIGHT       (GAME_HEIGHT + 2)
#define WIDTH        (HEIGHT * 2)
#define ACTUAL_WIDTH HEIGHT

#define MAP_WIDTH(x) (x * 2)

#define SNAKE_INIT 3

typedef struct GameCore* Game;

typedef enum Direction
{
    up    = 0,
    down  = 1,
    left  = 2,
    right = 3
} Direction;

typedef enum Status
{
    Alive  = 1,
    Dead   = 2,
    Player = 4,
    AI     = 8
} Status;

typedef struct Snake
{
    int       length;
    int       speed;
    COORD     head;
    Direction dir;
    Status    status;
    void (*Turn)(Game game, Direction dir);
    void (*Move)(Game game);
} Snake;

struct GameCore
{
    Screen    screen;
    Snake     snake;
    int       score;
    short     map[HEIGHT][WIDTH];
    COORD     food;
    COORD     history;
    ULONGLONG clock[3];
    void (*Update)(Game game);
};

Game NewGame();

void Restart(Game* game);

void DisplayStartMenu();

void freeGame(Game game);