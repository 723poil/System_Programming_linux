#include <unistd.h>
#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <aio.h>
#include <string.h>
#include <termios.h>

#define MESSAGE "hello"
#define BLANK   "     "

int row = 10;
int col = 0;
int dir = 1;
int delay = 200;
int done = 0;

struct aiocb kbcbuf;

void set_crmode(void);
void set_noecho(void);
void tty_mode(int);
int set_ticker(int);

int main() {

    void on_alarm(int);
    void on_input(int);
    void setup_aio_buffer();

    initscr();
    tty_mode(0);
    set_crmode();
    set_noecho();
    clear();

    signal(SIGIO, on_input);
    setup_aio_buffer();
    aio_read(&kbcbuf);

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

void on_input(int snum) {
    
    int c;
    char *cp = (char*)kbcbuf.aio_buf;

    if (aio_error(&kbcbuf) != 0 ) perror("reading failed");
    else {
	if (aio_return(&kbcbuf) == 1) {
	    c = *cp;
	    if (c == 'Q' || c == EOF) done = 1;
	    else if ( c == ' ' ) dir = -dir;
	}
    }

    aio_read(&kbcbuf);
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

void setup_aio_buffer() {

    static char input[1];

    kbcbuf.aio_fildes = 0;
    kbcbuf.aio_buf = input;
    kbcbuf.aio_nbytes = 1;
    kbcbuf.aio_offset = 0;

    kbcbuf.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
    kbcbuf.aio_sigevent.sigev_signo = SIGIO;
}

