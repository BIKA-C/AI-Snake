#include "../include/Game.h"
#include <conio.h>
#include <time.h>

void Quit(Game game)
{
    freeGame(game);
    exit(EXIT_SUCCESS);
}

int main()
{
    srand((unsigned int)time(NULL));

    DisplayStartMenu();
    Game        game = NewGame();

    while (true)
    {
        while ((game->snake.status & Alive) == Alive)
        {
            game->Update(game);
            while (_kbhit())
            {
                char c = _getch();
                switch (c)
                {
                    case 'W':
                    case 'w': game->snake.Turn(game, up); break;
                    case 'S':
                    case 's': game->snake.Turn(game, down); break;
                    case 'A':
                    case 'a': game->snake.Turn(game, left); break;
                    case 'D':
                    case 'd': game->snake.Turn(game, right); break;
                    case 'Q':
                    case 'q': Quit(game); break;
                    case 'r':
                    case 'R': Restart(&game); break;
                }
            }
        }
        CIcwcpy(game->screen->data[0], "   Died    R to Restart", 23);
        game->screen->Print(game->screen);
        switch (_getch())
        {
            case 'Q':
            case 'q': Quit(game); break;
            case 'r':
            case 'R': Restart(&game); break;
        }
    }
}