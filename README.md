# AI-Snake

>:warning: The AI feature will be added in the next few days

By William Chen

![demos](https://image.alkaid.cloud/Github/Snake/demo.gif)

## Description

> This is a Coding Club program

- Language written: C
- W A S D (R to restart and Q to quit)
- No flash screen
- Download the program `Snake.exe` in the [Release](https://github.com/BIKA-C/AI-Snake/tree/main/Release) folder or [here]()
- The `AI.exe` is not finished, so don't play with it

### To Run

> The terminal program I used is [Windows Terminal](https://www.microsoft.com/en-ca/p/windows-terminal/9n0dx20hk701?activetab=pivot:overviewtab)

- Open terminal in the [Release](https://github.com/BIKA-C/AI-Snake/tree/main/Release) folder
- Run `./Snake.exe`

## To Compile Yourself

- Clone the project
- Open the terminal in the project folder
- Run

``` shell
gcc Snake.c ConsoleScreen.c Game.c -D_CRT_SECURE_NO_WARNINGS -o '../Release/Snake.exe' -std=c11
```
