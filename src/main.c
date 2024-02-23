// STD libs include
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Other libs include
#include <curses.h>


// WARNING: NCURSES USES (Y;X), not (X;Y)
int main(int argc, char **argv)
{
    (void) argc;
    (void) argv;
    // Game setup
    initscr();
    int width, height;
    getmaxyx(stdscr, height, width);
    char *welcome_text = "Hello From Matter Dimentions Project!";
    move(height/2, width/2 - (int)strlen(welcome_text)/2);
    printw("%s", welcome_text);
    getch();
    endwin();
    printf("%dx%d\n", width, height);
    return 0;
}
