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

#pragma once
#include <windows.h>

#define BLACK        0
#define BLUE         1
#define GREEN        2
#define AQUA         3
#define RED          4
#define PURPLE       5
#define YELLOW       6
#define WHITE        7
#define GREY         8
#define LIGHT_BLUE   9
#define LIGHT_GREEN  10
#define LIGHT_AQUA   11
#define LIGHT_RED    12
#define LIGHT_PURPLE 13
#define LIGHT_YELLOW 14
#define BRIGHT_WHITE 15

#define COLOR(foreground, background) (background << 4 | foreground)

// Screen object
typedef struct ConsoleScreen* Screen;
struct ConsoleScreen
{
    int         bufIndex;
    HANDLE      optbuf[2];
    CHAR_INFO** data;
    int         height;
    int         width;

    void (*Print)(Screen);
};

// free a Screen object
void freeScreen(Screen);

// copy a string to a Screen.data row: YourScreenName.data[i]
// - CHAR_INFO* dest: YourScreenName.data[i]
// - char* source: "your string"
// - int length: the length of the string you want to copy
void CIcwcpy(CHAR_INFO* dest, char* source, int length);

// create the screen
Screen CreateTerminalScreen(int height, int width);