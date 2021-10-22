#include <unistd.h>
#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>

#define MESSAGE "hello"
#define BLANK   "     "

int row = 10;
int col = 0;
int dir = 1;
int delay = 200;
int done = 0;

void set_crmode(void);
void set_noecho(void);
void tty_mode(int);
int set_ticker(int);

int main() {

    void on_alarm(int);
    void on_input(int);
    void enable_kbd_signals();

    initscr();
    tty_mode(0);
    set_crmode();
    set_noecho();
    clear();

    signal(SIGIO, on_input);
    enable_kbd_signals();
    signal(SIGALRM, on_alarm);
    set_ticker(delay);

    move(row, col);
    addstr( MESSAGE );

    while(!done) pause();

    tty_mode(1);
    endwin();
}

void set_crmode() {

    struct termios ttystate;

    tcgetattr(0, &ttystate);
    ttystate.c_lflag &= ~ICANON;
    ttystate.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &ttystate);
}

void set_noecho() {

    struct termios ttystate;

    tcgetattr(0, &ttystate);
    ttystate.c_lflag &= ~ECHO;
    tcsetattr(0, TCSANOW, &ttystate);
}

void tty_mode(int how) {

    static struct termios original_mode;

    if (how == 0) {
	tcgetattr(0, &original_mode);
    }
    else {
	tcsetattr(0, TCSANOW, &original_mode);
    }
}

void on_input(int signum) {
    
    int c = getch();

    if (c == 'Q' || c == EOF ) done = 1;
    else if ( c == ' ' ) dir = -dir;
}

void on_alarm(int signum) {

    signal(SIGALRM, on_alarm);
    mvaddstr( row, col, BLANK );
    col += dir;
    mvaddstr( row, col, MESSAGE );
    refresh();

    if (dir == -1 && col <= 0 ) dir = 1;
    else if ( dir == 1 && col+strlen(MESSAGE) >= COLS ) dir = -1;
}

void enable_kbd_signals() {

    int fd_flags;

    fcntl(0, F_SETOWN, getpid());
    fd_flags = fcntl(0, F_GETFL);
    fcntl(0, F_SETFL, (fd_flags|O_ASYNC));
}


