// STD libs include
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <pthread.h>
#include <stddef.h>
#include <uchar.h>
#include <locale.h>
#include <wchar.h>
#include <assert.h>

// Other libs include
#define _XOPEN_SOURCE 700
#include <curses.h>

// MD imports
#include "types.h"

#define string char*

string box_left_up_corner    = "+";
string box_left_down_corner  = "+";
string box_right_up_corner   = "+";
string box_right_down_corner = "+";
string box_horisontal_bar    = "-";
string box_vertical_bar      = "|";

bool utf8_enabled = false;

size_t dimention_selected = 0;

#define DIMENTIONS_TOTAL 10
Dimention dimentions[DIMENTIONS_TOTAL] = {
    {.id=1,.earn_rate=1   ,.tick_speed=1, .amount=2},
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

GameProgress game_progress = {0};
#include "calculation.c"
// WARNING: NCURSES USES (Y;X), not (X;Y)
typedef enum loadingScreenTextEnum {LSTE_HELLO, LSTE_PRESS_ANY} lSTE;
string loadingScreenText[] = {
    [LSTE_HELLO] = "Hello From Matter Dimentions Project!",
    [LSTE_PRESS_ANY] = "Press any button to continue..."
};

void drawRect(WINDOW *window, Vector2 pos, size_t lines, size_t columns)
{
    if(window==NULL){
        mvaddstr(pos.y,         pos.x,           box_left_up_corner);
        mvaddstr(pos.y+lines+1, pos.x+columns+1, box_right_down_corner);
        mvaddstr(pos.y+lines+1, pos.x,           box_left_down_corner);
        mvaddstr(pos.y,         pos.x+columns+1, box_right_up_corner);
        for (size_t i = 0; i < columns; i++) {
            mvaddstr(pos.y,         pos.x+1+i, box_horisontal_bar);
            mvaddstr(pos.y+lines+1, pos.x+1+i, box_horisontal_bar);
        }
        for (size_t i = 0; i < lines; i++) {
            mvaddstr(pos.y+1+i, pos.x,           box_vertical_bar);
            mvaddstr(pos.y+1+i, pos.x+columns+1, box_vertical_bar);
        }
        move(0,0);
        return;
    }
    mvwaddstr(window, pos.y,         pos.x,           box_left_up_corner);
    mvwaddstr(window, pos.y+lines+1, pos.x+columns+1, box_right_down_corner);
    mvwaddstr(window, pos.y+lines+1, pos.x,           box_left_down_corner);
    mvwaddstr(window, pos.y,         pos.x+columns+1, box_right_up_corner);
    for (size_t i = 0; i < columns; i++) {
        mvwaddstr(window, pos.y,         pos.x+1+i, box_horisontal_bar);
        mvwaddstr(window, pos.y+lines+1, pos.x+1+i, box_horisontal_bar);
    }
    for (size_t i = 0; i < lines; i++) {
        mvwaddstr(window, pos.y+1+i, pos.x,           box_vertical_bar);
        mvwaddstr(window, pos.y+1+i, pos.x+columns+1, box_vertical_bar);
    }
    move(0,0);
}

void drawTextRectWin(WINDOW *window, const string str, Vector2 pos, size_t lines, size_t columns)
{
    drawRect(window, pos, lines, columns);
    if(window==NULL){
        mvaddstr(pos.y+1, pos.x+1, str);
    }
    mvwaddstr(window, pos.y+1,pos.x+1, str);
    move(0,0);
}

void drawTextRectAuto(WINDOW *window, const string str, Vector2 pos)
{
    drawTextRectWin(window, str, pos, 1, strlen(str));
}

void drawTextVec(WINDOW *window, const string str, Vector2 pos)
{
    if(window==NULL){
        mvaddstr(pos.y+1,pos.x+1, str);
        return;
    }
    mvwaddstr(window, pos.y+1,pos.x+1, str);
}

Vector2 getCenter(WINDOW *window)
{
    int width, height;
    if(window==NULL){
        getmaxyx(window, height, width);
    }
    getmaxyx(stdscr, height, width);
    return (Vector2){height/2, width/2};
}

string shift_args(int *argc, string **argv)
{
    *argc-=1;
    *argv+=1;
    return *(*argv-1);
}

void print_usage(const string program_name){
    fprintf(stderr, "usage:\n%s [arguments]\n", program_name);
    fprintf(stderr, "arguments:\n");
    fprintf(stderr, "\t-utf8 : enables utf-8 support instead of ascii\n");
}

int main(int argc, string *argv)
{
    // Game setup
    setlocale(LC_ALL,"");
    string program_name = shift_args(&argc, &argv);
    while(argc!=0){
        string argument = shift_args(&argc, &argv);
        if(strcmp(argument, "-h")==0){
            print_usage(program_name);
            exit(69);
        } else if(strcmp(argument, "-utf8")==0){
            utf8_enabled = true;
        }
    }
    // TODO: add utf-8 markings for box boundaries
    if(utf8_enabled){
        box_vertical_bar      = "│";
        box_horisontal_bar    = "─";
        box_left_up_corner    = "╭";
        box_right_up_corner   = "╮";
        box_left_down_corner  = "╰";
        box_right_down_corner = "╯";
    }

    WINDOW *main_window = initscr();
    int width, height;
    getmaxyx(stdscr, height, width);
    curs_set(0);
    noecho();
    cbreak();
    // Loading menu
    Vector2 pos = {width/2 - strlen(loadingScreenText[LSTE_HELLO])/2-1, height/2-1};
    drawTextRectAuto(NULL, loadingScreenText[LSTE_HELLO], pos);
    drawTextVec(NULL, loadingScreenText[LSTE_PRESS_ANY],
                (Vector2){
                    width/2 - strlen(loadingScreenText[LSTE_PRESS_ANY])/2,
                    height-5
                });
    WINDOW *game_window = newwin(height, width*2/3, 0, width/3);
    keypad(game_window, TRUE);
    WINDOW *other_window = newwin(height, width/3, 0, 0);
    // Nodelay initialisation
    nodelay(main_window, true);
    nodelay(game_window, true);
    nodelay(other_window, true);
    keypad(main_window, TRUE);
    keypad(game_window, TRUE);
    keypad(other_window, TRUE);
    //-Nodelay-initialisation-
    Vector2 game_window_center = getCenter(game_window);
    int char_pressed = getch();
    // Game start
    clear();
    atomic_exchange(&game_progress.dims_unlocked, 3);
    atomic_exchange(&game_progress.matter_amount, 0);
    pthread_t calculation_thread_id;
    pthread_create(&calculation_thread_id, NULL, calculation_loop, NULL);
    char *tmpstr = malloc(strlen("Dim")+100);
    while((char_pressed = wgetch(game_window))!='q'){
        for (size_t i = 0; i < atomic_load(&game_progress.dims_unlocked) && i<DIMENTIONS_TOTAL; i++) {
            Dimention cur_dim = {
                .id         = atomic_load(&dimentions[i].id),
                .earn_rate  = atomic_load(&dimentions[i].earn_rate),
                .tick_speed = atomic_load(&dimentions[i].tick_speed),
                .amount     = atomic_load(&dimentions[i].amount)
            };
            sprintf(tmpstr, "Dim %zu: %zu per %zu second | %zu", cur_dim.id, cur_dim.earn_rate, cur_dim.tick_speed, cur_dim.amount);
            Vector2 pos = {game_window_center.x-strlen(tmpstr)/2,
                           (height-5)*i/DIMENTIONS_TOTAL+5};
            if(i==dimention_selected){
                wattron(game_window, A_STANDOUT);
            } else {
                wattroff(game_window, A_STANDOUT);
            }
            drawTextRectAuto(game_window, tmpstr, pos);
        }
        wattroff(game_window, A_STANDOUT);
        sprintf(tmpstr, "Matter amount: %zu", atomic_load(&game_progress.matter_amount));
        mvwaddstr(game_window, 2, game_window_center.x-5,"Game Window");
        mvwaddstr(other_window, 1, 1, tmpstr);
        box(game_window,0,0);
        box(other_window,0,0);
        wrefresh(game_window);
        wrefresh(other_window);
        size_t tmp_dims_unlocked = atomic_load(&game_progress.dims_unlocked);
        switch(char_pressed){
            case KEY_UP:
                dimention_selected = (dimention_selected==0)?tmp_dims_unlocked-1:dimention_selected - 1;
                break;
            case KEY_DOWN:
                dimention_selected = (dimention_selected + 1)%tmp_dims_unlocked;
                break;
            default:
                break;
        }
    }
    free(tmpstr);
    endwin();
    printf("%dx%d\n", width, height);
    return 0;
}
