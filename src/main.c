// STD libs include
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// Other libs include
#include <curses.h>

// MD imports
#include "types.h"

void drawRect(Vector2 pos, size_t lines, size_t columns)
{
    mvaddch(pos.y, pos.x,'+');
    mvaddch(pos.y+lines+1, pos.x+columns+1, '+');
    mvaddch(pos.y+lines+1, pos.x,'+');
    mvaddch(pos.y, pos.x+columns+1, '+');
    for (size_t i = 0; i < columns; i++) {
        mvaddch(pos.y, pos.x+1+i, '-');
        mvaddch(pos.y+lines+1, pos.x+1+i,'-');
    }
    for (size_t i = 0; i < lines; i++) {
        mvaddch(pos.y+1+i, pos.x, '|');
        mvaddch(pos.y+1+i, pos.x+columns+1,'|');
    }
    move(0,0);
}

void drawTextRect(const char *str, Vector2 pos, size_t lines, size_t columns)
{
    drawRect(pos, lines, columns);
    mvaddstr(pos.y+1,pos.x+1, str);
    move(0,0);
}

void drawTextRectAuto(const char *str, Vector2 pos)
{
    drawTextRect(str, pos, 1, strlen(str));
}

void drawTextVec(const char *str, Vector2 pos)
{
    mvaddstr(pos.y+1,pos.x+1, str);
}

Vector2 getCenter(void)
{
    int width, height;
    getmaxyx(stdscr, height, width);
    return (Vector2){height/2, width/2};
}

#define DIMENTIONS_TOTAL 10
Dimention dimentions[DIMENTIONS_TOTAL] = {
    {.id=1,.earn_rate=1   ,.tick_speed=1, .amount=0},
    {.id=2,.earn_rate=10  ,.tick_speed=1, .amount=0},
    {.id=3,.earn_rate=100 ,.tick_speed=1, .amount=0},
    {.id=4,.earn_rate=1000,.tick_speed=1, .amount=0},
    {.id=5,.earn_rate=1000,.tick_speed=1, .amount=0},
    {.id=6,.earn_rate=1000,.tick_speed=1, .amount=0},
    {.id=7,.earn_rate=1000,.tick_speed=1, .amount=0},
    {.id=8,.earn_rate=1000,.tick_speed=1, .amount=0},
    {.id=9,.earn_rate=1000,.tick_speed=1, .amount=0},
    {.id=10,.earn_rate=1000,.tick_speed=1, .amount=0},
};

// WARNING: NCURSES USES (Y;X), not (X;Y)
typedef enum loadingScreenTextEnum {LSTE_HELLO, LSTE_PRESS_ANY} lSTE;
char *loadingScreenText[] = {
    [LSTE_HELLO] = "Hello From Matter Dimentions Project!",
    [LSTE_PRESS_ANY] = "Press any button to continue..."
};
GameProgress game_progress = {0};
int main(int argc, char **argv)
{
    (void) argc;
    (void) argv;
    // Game setup
    initscr();
    int width, height;
    getmaxyx(stdscr, height, width);
    // debugging coordinates print
    for(int i=1; i<height+1; i++){mvaddch(i, 0, '0'+i%10);}for(int i=1; i<width+1; i++){mvaddch(0, i, '0'+i%10);}
    // Loading menu
    Vector2 pos = {width/2 - strlen(loadingScreenText[LSTE_HELLO])/2-1, height/2-1};
    drawTextRectAuto(loadingScreenText[LSTE_HELLO], pos);
    drawTextVec(loadingScreenText[LSTE_PRESS_ANY],
                (Vector2){
                    width/2 - strlen(loadingScreenText[LSTE_PRESS_ANY])/2,
                    height-5
                });
    char char_pressed = getch();
    // Game start
    clear();
    game_progress.dims_unlocked = 0;
    game_progress.matter_amount = 0;
    while((char_pressed = getch())!='q'){
        for (size_t i = 0; i < game_progress.dims_unlocked && i<DIMENTIONS_TOTAL; i++) {
            char *tmpstr = malloc(strlen("Dim")+100);
            sprintf(tmpstr, "Dim %zu: %zu per %zu second | %zu",
                    dimentions[i].id, dimentions[i].earn_rate, dimentions[i].tick_speed, dimentions[i].amount);
            Vector2 pos = {width/2-strlen(tmpstr)/2, height*i/DIMENTIONS_TOTAL};
            drawTextRectAuto(tmpstr, pos);
        }
        for (size_t i = 0; i < game_progress.dims_unlocked && i < DIMENTIONS_TOTAL; i++) {
            dimentions[i].amount += dimentions[i].earn_rate;
        }
        game_progress.dims_unlocked++;
    }
    endwin();
    printf("%dx%d\n", width, height);
    return 0;
}
