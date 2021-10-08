/*
file        : play_again1.c
author      : 2017110051_이상협
datetime    : 2021-10-08 10:40
description : Code to ask if user want to do another transaction
*/

#include <stdio.h>
#include <termios.h> // header file with structure contaning setting information

/* Since it can be changed, 
   it defines it so that it can be easily changed later. */
#define QUESTION "Do you want another transaction"

int get_response(char *);
void set_crmode(void);
void tty_mode(int);

int main(void) {

    int response;

    tty_mode(0);                        // save tty mode
    set_crmode();                       // set chr-by-chr mode
    response = get_response(QUESTION);  // get some answer
    tty_mode(1);                        // restore tty mode

    return response;
}

int get_response(char* question) {
    // ask question and wait for a answer
    int input;
    
    printf("%s (y/n)?", question);

    while(1) {
            switch( input = getchar() ) {
                case 'y':
                case 'Y': return 0;  // y, Y -> return 0
                case 'n':
                case 'N':
                case EOF: return 1;  // n, N, EOF -> return 1
                default:             // other answer -> reinput
                    printf("\ncannot understand %c, ", input);
                    printf("Please type y or no\n");
            }
    }
}

void set_crmode(void) {
    // put fd 0 (stdin) into ttystate
    struct termios ttystate;

    tcgetattr(0, &ttystate);           // read current setting
    ttystate.c_lflag &= ~ICANON;       // no buffering -> don`t need to press enter key
    ttystate.c_cc[VMIN] = 1;           // get one char at a time
    tcsetattr(0, TCSANOW, &ttystate);  // now setting
}

void tty_mode(int how) {

    /* if it goes out of the function, the address value of
    the local variable disappears, so it is declared a global variable */
    static struct termios original_mode;
    
    if ( how == 0 ) {
        tcgetattr(0, &original_mode);  // get current setting
    }
    else {
        tcsetattr(0, TCSANOW, &original_mode); // set original setting
    }
}
