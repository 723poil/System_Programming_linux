#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <string.h>
#include <termios.h>

#define MESSAGE "hello"
#define BLANK   "     "

int row;
int col;
int dir;

void set_crmode();
void set_noecho();
void tty_mode(int);
int set_ticker(int);

int main() {

    int delay;
    int ndelay;
    int c;
    void move_msg(int);

    tty_mode(0);
    initscr();
    set_crmode();
    set_noecho();
    clear();

    row = 10;
    col = 0;
    dir = 1;
    delay = 200;

    move(row, col);
    addstr(MESSAGE);
    signal(SIGALRM, move_msg);
    set_ticker(delay);

    while(1) {
	ndelay = 0;
	c = getchar();
	if ( c == 'Q') break;
	if ( c == ' ') dir = -dir;
	if ( c == 'f' && delay > 0 ) ndelay = delay/2;
	if ( c == 's' ) ndelay = delay * 2;
	if ( ndelay > 0 ) set_ticker(delay = ndelay);
    }

    endwin();
    tty_mode(1);

    return 0;
}

void move_msg(int signum) {

    signal(SIGALRM, move_msg);
    move(row, col);
    addstr(BLANK);
    col += dir;
    move(row, col);
    addstr(MESSAGE);
    refresh();

    if (dir == -1 && col <= 0) {
	dir = 1;
    }
    else if (dir == 1 && col + strlen(MESSAGE) >= COLS) {
	dir = -1;
    }
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
    ttystate.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &ttystate);
}

void tty_mode(int how) {

    static struct termios original_mode;

    if(how == 0) {
	tcgetattr(0, &original_mode);
    }
    else {
	tcsetattr(0, TCSANOW, &original_mode);
    }
}
