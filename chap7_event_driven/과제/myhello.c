/*
file        : myhello.c
author      : 2017110051_이상협
datetime    : 2021-10-15 11:50
description : bounce a message back and forth across the screen
              the set_ticker.c file allows it to operate in units smaller than seconds.
*/
#include <stdio.h>
#include <curses.h> // for implement animation
#include <unistd.h> // pause operation
#include <stdlib.h> // malloc operation
#include <signal.h> // siganl operation
#include <string.h> // strlen operation

#define LEFTEDGE 10
#define RIGHTEDGE 30
#define ROW 10

void m_animation();

char* message;                    // to output string
char blank[] = "          ";      // erase
int dir = 1;                      // direction
int pos = LEFTEDGE;               // current position

int main() {

    message = (char*)malloc(sizeof(char)*(10)); // allocate

    printf("Enter a message less than 10 in length!\n"); // limit
    scanf("%[^\n]", message);                            // enter the string to output

    if (strlen(message) >= 10) {
	printf("Too long string!\n"); // if the size exceeds the limit, output
	exit(1);
    }

    signal(SIGALRM, m_animation);     // register signal

    initscr();
    clear();

    m_animation();

    if(set_ticker(250) == -1) {
        perror("set_ticker");
    }
    else {
	while(1) {
            pause();                  // block
        }
    }
}

void m_animation() {

    move(ROW, pos);
    addstr( message );             // draw string
    move(LINES-1, COLS-1);         // park the cursor

    refresh();                     // show string

    move(ROW, pos);                // erase string
    addstr(blank);

    pos += dir;                    // advance position

    if(pos >= RIGHTEDGE) dir = -1; // check for bounce
    if(pos <= LEFTEDGE) dir = 1;
}
