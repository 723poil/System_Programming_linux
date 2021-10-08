/*
file        : play_again3.c
author      : 2017110051_이상협
datetime    : 2021-10-08 11:53
description : use nonblocking mode for timeouts
*/

#include <stdio.h>
#include <termios.h>
#include <fcntl.h>    // file control
#include <string.h>   // get_ok_char operation
#include <ctype.h>    // tolower operation
#include <unistd.h>   // sleep opeartion

#define ASK "Do you want another transaction"
#define TRIES 3                  // maximum number of attempts
#define SLEEPTIME 2              // 2 second
#define BEEP putchar('\a')       // sound output

int get_response(char *, int);
void set_cr_noecho_mode(void);
void set_nodelay_mode(void);     // nodelay operation
void tty_mode(int);
int get_ok_char(void);           // character return that meets the desired criteria

int main(void) {

    int response;

    tty_mode(0);
    set_cr_noecho_mode();
    set_nodelay_mode();                   // no input EOF
    response = get_response(ASK, TRIES);  // add try
    tty_mode(1);

    return response;
}

int get_response(char* question, int maxtries) {
    
    int input;

    printf("%s (y/n)?", question);
    fflush(stdout);                          // to empty the buffer

    while(1) {      
            sleep(SLEEPTIME);                // wait input
            input = tolower(get_ok_char());
            if ( input == 'y' ) return 0;
            if ( input == 'n' ) return 1;
            if ( maxtries-- == 0 ) return 2; // time out
            BEEP;                            // sound output
    }
}

int get_ok_char() {

    int c;
    while( ( c = getchar() ) != EOF && strchr("yYnN", c) == NULL );

    return c;
}

void set_cr_noecho_mode() {

    struct termios ttystate;

    tcgetattr(0, &ttystate);
    ttystate.c_lflag &= ~ICANON;
    ttystate.c_lflag &= ~ECHO;
    ttystate.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &ttystate);
}

void set_nodelay_mode() {

    int termflags;
    termflags = fcntl(0, F_GETFL);  // read current setting
    termflags |= O_NDELAY;          // set on nodelay - even if getchar is empty, return and proceed
    fcntl(0, F_SETFL, termflags);   // setting
}

void tty_mode(int how) {

    static struct termios original_mode;
    static int original_flags;

    if ( how == 0 ) {
        tcgetattr(0, &original_mode);
        original_flags = fcntl(0, F_GETFL);
    }
    else {
        tcsetattr(0, TCSANOW, &original_mode);
        fcntl(0, F_SETFL, original_flags);
    }
}