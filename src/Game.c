#include "../include/Game.h"
#include <conio.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void Display(Game game);
void NewFood(Game game);
void Update(Game game);
void Move(Game game);
void Turn(Game game, Direction dir);

// mark a spot on the map
void LabelMap(Game game, int r, int c, int label)
{
    game->map[r][c]     = label;
    game->map[r][c + 1] = label;
}

// init game
Game NewGame()
{
    Game this = calloc(1, sizeof(struct GameCore));

    // init snake
    this->snake.Move   = Move;
    this->snake.Turn   = Turn;
    this->snake.status = Alive | Player;
    this->snake.speed  = 150;
    this->snake.length = SNAKE_INIT;
    this->snake.dir    = up;
    this->snake.head   = (COORD) {MAP_WIDTH((rand() % (GAME_WIDTH - this->snake.length - 2)) + this->snake.length), (rand() % (GAME_HEIGHT - this->snake.length - 2)) + this->snake.length};

    // init game
    this->score    = 0;
    this->history  = this->snake.head;
    this->clock[0] = GetTickCount64();
    this->clock[1] = this->clock[0] + this->snake.speed;
    this->clock[2] = 0;
    this->screen   = CreateTerminalScreen(HEIGHT + 1, WIDTH);
    this->Update   = Update;

    // create map
    for (int r = 0; r < HEIGHT; r++)
        for (int c = 0; c < WIDTH; c += 2)
        {
            if ((c == 0) || ((c % WIDTH) == WIDTH - MAP_WIDTH(1)))
                LabelMap(this, r, c, BORDER);
            else if (r == 0 || r == HEIGHT - 1)
                LabelMap(this, r, c, BORDER);
            else
                LabelMap(this, r, c, EMPTY);
        }

    // create snake
    for (int i = 1; i <= 3; i++)
    {
        switch (this->snake.dir)
        {
            case up: LabelMap(this, this->snake.head.Y + (i - 1), (this->snake.head.X), i); break;
            case down: LabelMap(this, this->snake.head.Y - (i - 1), (this->snake.head.X), i); break;
            case left: LabelMap(this, this->snake.head.Y, (this->snake.head.X + (i - 1)), i); break;
            case right: LabelMap(this, this->snake.head.Y, (this->snake.head.X - (i - 1)), i); break;
        }
    }

    // create food
    NewFood(this);
    return this;
}

// create a new food
void NewFood(Game game)
{
    // all empty spots
    COORD empty[GAME_AREA];
    memset(empty, 0x0, sizeof(COORD) * GAME_AREA);

    // store a spot if it is empty
    int index = 0;
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j += MAP_WIDTH(1))
            if (game->map[i][j] == EMPTY)
            {
                empty[index].X = j;
                empty[index].Y = i;
                index++;
            }

    int random = rand() % index;

    game->food.X = empty[random].X;
    game->food.Y = empty[random].Y;
    LabelMap(game, game->food.Y, game->food.X, FOOD);
}

// test collision
bool Collide(Game this)
{
    // if the head coord is not empty or food, return false
    return ((this->map[this->snake.head.Y][this->snake.head.X] != EMPTY && this->map[this->snake.head.Y][this->snake.head.X] != FOOD) || (this->map[this->snake.head.Y][this->snake.head.X + 1] != EMPTY && this->map[this->snake.head.Y][this->snake.head.X + 1] != FOOD));
}

// test score
bool Score(Game this)
{
    // if the head coord is food, return true
    return ((this->snake.head.Y == this->food.Y && this->snake.head.X == this->food.X) || (this->snake.head.Y == this->food.Y && this->snake.head.X + 1 == this->food.X));
}

// return the opposite dir of a given dir
Direction Opposite(Direction dir)
{
    switch (dir)
    {
        case up: return down;
        case down: return up;
        case right: return left;
        case left: return right;
    }
}

// search a given label in the surronding area of a given spot
COORD Search(Game game, COORD start, int label)
{
    if (game->map[start.Y + 1][start.X] == label)
        return (COORD) {start.X, start.Y + 1};

    else if (game->map[start.Y - 1][start.X] == label)
        return (COORD) {start.X, start.Y - 1};

    else if (game->map[start.Y][start.X + MAP_WIDTH(1)] == label)
        return (COORD) {start.X + MAP_WIDTH(1), start.Y};

    else if (game->map[start.Y][start.X - MAP_WIDTH(1)] == label)
        return (COORD) {start.X - MAP_WIDTH(1), start.Y};

    // if the label does not exist, return (0, 0)
    return (COORD) {0, 0};
}

bool PointCompare(COORD p1, COORD p2)
{
    return (p1.X == p2.X && p1.Y == p2.Y);
}

// set the direction of the snake
void Turn(Game this, Direction dir)
{
    // if the new dir is same as the old or the opposite of the old, return
    // if receive a input that is before the update, return
    if (PointCompare(this->history, this->snake.head)) return;
    if (dir == this->snake.dir || dir == Opposite(this->snake.dir)) return;
    this->snake.dir = dir;
    this->history   = this->snake.head;
}

// there are some weird random errors that I could not debug and fix
// this function check the logic of the map and fix any error
// Nevermind, bug fixed
void ErrorCorrection(Game this)
{
    if (this->map[this->food.Y][this->food.X] != FOOD)
    {
        this->map[this->food.Y][this->food.X] = FOOD;
        this->snake.length++;
        this->score++;
    }

    if (this->score != this->snake.length - SNAKE_INIT)
        this->score = this->snake.length - SNAKE_INIT;

    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j += MAP_WIDTH(1))
        {
            if (i == this->food.Y && j == this->food.X) continue;
            if (this->map[i][j] == FOOD)
                LabelMap(this, i, j, EMPTY);
        }
}

// update the screen based on the speed of the snake
void Update(Game game)
{
    game->clock[2] = GetTickCount64();
    if (game->clock[2] < game->clock[1]) return;
    Move(game);
    // ErrorCorrection(game); // Nevermind, bug fixed
    Display(game);
    game->clock[1] += game->snake.speed;
}

// move one block towards the current direction
void Move(Game this)
{
    COORD save = {this->snake.head.X, this->snake.head.Y};

    // update head
    switch (this->snake.dir)
    {
        case up: this->snake.head.Y--; break;
        case down: this->snake.head.Y++; break;
        case left: this->snake.head.X -= MAP_WIDTH(1); break;
        case right: this->snake.head.X += MAP_WIDTH(1); break;
    }

    // test collision
    if (Collide(this))
    {
        Status old         = (this->snake.status & AI) == AI ? AI : Player;
        this->snake.status = old | Dead;
        return;
    }

    // mark the new position of the head
    LabelMap(this, this->snake.head.Y, this->snake.head.X, 1);

    // update the body
    COORD current = save;
    COORD end;
    for (int i = 2; i <= this->snake.length; i++)
    {
        if (i != 2)
            current = Search(this, current, i - 1);
        // next = Search(this, current, 1);
        LabelMap(this, current.Y, current.X, i);
        if (i == this->snake.length)
            end = Search(this, current, i);
    }

    // test food
    bool score = false;
    if (Score(this))
    {
        this->score++;
        this->snake.length++;
        this->snake.speed = this->snake.speed - 2 >= 10 ? this->snake.speed - 2 : 10;
        score             = true;
    }

    // if the length of the snake is one:
    if (this->snake.length == 1)
    {
        if (score)
        {
            NewFood(this);
            return;
        }
        LabelMap(this, save.Y, save.X, 0);
        return;
    }

    // cut the tail
    if (end.X != 0) LabelMap(this, end.Y, end.X, 0);
    if (score) NewFood(this);
    return;
}

// present the map
void Display(Game this)
{
    char temp[WIDTH] = {0};
    sprintf(temp, "   %s                          Score: %d", ((this->snake.status & AI) == AI) ? "AI    " : "Player", this->score);
    CIcwcpy(this->screen->data[0], temp, WIDTH);
    for (int r = 0; r < HEIGHT; r++)
        for (int c = 0; c < WIDTH; c++)
        {
            this->screen->data[r + 1][c].Char.UnicodeChar = 0x3000;
            if (this->map[r][c] == EMPTY)
                this->screen->data[r + 1][c].Attributes = COLOR(BLACK, BLACK);
            else if (this->map[r][c] == FOOD)
                this->screen->data[r + 1][c].Attributes = COLOR(WHITE, GREEN);
            else if (this->map[r][c] == BORDER)
                this->screen->data[r + 1][c].Attributes = COLOR(WHITE, YELLOW);
            else
                this->screen->data[r + 1][c].Attributes = COLOR(WHITE, BLUE);
        }
    this->screen->Print(this->screen);
}

// memory recycle
void freeGame(Game game)
{
    freeScreen(game->screen);
}

// display the menu
void DisplayStartMenu()
{
    Screen menu = CreateTerminalScreen(9, 40);
    CIcwcpy(menu->data[0], "----------------------------------------", 40);
    CIcwcpy(menu->data[1], "-------     // SNAKE Game //     -------", 40);
    CIcwcpy(menu->data[2], "-------       R to Restart       -------", 40);
    CIcwcpy(menu->data[3], "-------         Q to Quit        -------", 40);
    CIcwcpy(menu->data[4], "-------          W A S D         -------", 40);
    CIcwcpy(menu->data[5], "-------       Space for AI       -------", 40);
    CIcwcpy(menu->data[6], "-------  Press any key to START  -------", 40);
    CIcwcpy(menu->data[7], "-------      BY William Chen     -------", 40);
    CIcwcpy(menu->data[8], "----------------------------------------", 40);
    menu->Print(menu);
    _getch();
    freeScreen(menu);
}

// new game
void Restart(Game* game)
{
    freeGame(*game);
    *game = NewGame();
}