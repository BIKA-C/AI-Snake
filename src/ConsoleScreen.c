// A set of "handy" deeply customized console print APIs
// Solve the following issue:
//  1. Flash screen at high refresh rate. (use double buffer)
//  2. Coloring
//      - This "Screen" object treat each character spot as a pixel. Each pixel carries
//        a 'char' or 'unicode char' data and their color info (both foreground and background)
//  3. currently does not provide coloring API, you can take a look of the code if you are interested
// 
//  <HOW to USE>
//  1. Screen YourScreenName = CreateTerminalScreen(heightOfScreen, widthOfScreen);
//  2. Under your "Screen" object:
//      - 'data' is the your screen pixels that is the only thing you want to modify
//         use CIcwcpy to copy your string to a 'data' row
//  3. use YourScreenName.Print(YourScreenName); to present
// By William Chen

#include "../include/ConsoleScreen.h"
#include <stdbool.h>
#include <stdlib.h>

void WriteConsoleOutputCI(HANDLE hConsoleOutput, CHAR_INFO* data, DWORD nLength, COORD coord)
{
    DWORD byte;
    for (int i = 0; i < (int)nLength; i++, coord.X++)
    {
        WriteConsoleOutputCharacterW(hConsoleOutput, &data[i].Char.UnicodeChar, 1, coord, &byte);
        WriteConsoleOutputAttribute(hConsoleOutput, &data[i].Attributes, 1, coord, &byte);
    }
}

void Print(Screen this)
{
    this->bufIndex = ! this->bufIndex;
    COORD coord    = (COORD) {0, 0};
    for (int i = 0; i < this->height; i++, coord.Y++)
        WriteConsoleOutputCI(this->optbuf[this->bufIndex], this->data[i], this->width, coord);

    SetConsoleActiveScreenBuffer(this->optbuf[this->bufIndex]);
}

Screen CreateTerminalScreen(int height, int width)
{
    SetConsoleOutputCP(65001);

    Screen this    = calloc(1, sizeof(struct ConsoleScreen));
    this->bufIndex = 0;
    this->height   = height;
    this->width    = width;
    for (int i = 0; i < 2; i++)
    {
        this->optbuf[i] = CreateConsoleScreenBuffer(
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL,
            CONSOLE_TEXTMODE_BUFFER,
            NULL);
        SetConsoleCursorInfo(this->optbuf[i], &(CONSOLE_CURSOR_INFO) {1, false});
        FlushConsoleInputBuffer(this->optbuf[i]);
    }

    this->data = calloc(height, sizeof(CHAR_INFO*));
    for (int c = 0; c < height; c++)
        this->data[c] = calloc(width, sizeof(CHAR_INFO));

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            this->data[i][j].Attributes = COLOR(WHITE, BLACK);

    this->Print = Print;
    return this;
}

void freeScreen(Screen this)
{
    for (int i = 0; i < this->height; i++)
        free(this->data[i]);
    free(this->data);
}

void CIcwcpy(CHAR_INFO* dest, char* source, int length)
{
    for (int i = 0; i < length; i++)
        mbstowcs(&(dest[i].Char.UnicodeChar), &source[i], 1);
}