#include <stdio.h>
#include <signal.h>
#include <ncurses.h>
#include <termios.h>
#include <assert.h>

#define WIDTH 30
#define HEIGHT 10

int startx = 0;
int starty = 0;

void set_crmode();
void set_noecho();
void tty_mode(int);
void mouse_event();

int main(void) {

    
   // tty_mode(0);
   // set_crmode();
   // set_noecho();
    
    mouse_event();

    return 0;
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

void mouse_event() {
    WINDOW *menu_win;

    int ch;

    //init_pair(1, COLOR_WHITE, COLOR_WHITE);

    initscr();
    keypad(stdscr, TRUE);
    noecho();
    clear();
    cbreak();

    startx = (80 - WIDTH) / 2;
    starty = (24 - HEIGHT) / 2;

    menu_win = newwin(HEIGHT, WIDTH, starty, startx);

    attron(A_REVERSE);
    mvprintw(23, 1, "Click on Exit to quit (Works best in a virtual console)");
    refresh();
    attroff(A_REVERSE);

    mousemask(ALL_MOUSE_EVENTS, NULL);

    while(1) {
	ch = getch();
	if (ch == KEY_MOUSE) {
	    MEVENT event;
	    if(getmouse(&event) == OK) {
	    	if (event.bstate & BUTTON1_PRESSED) {
		    attron(A_REVERSE);
		    mvprintw(event.y, event.x, " ");
		    attroff(A_REVERSE);    
	            refresh();
		    //mvprintw(10, 10, event.x);
		    
	            //mvprintw(12, 10, event.y);
	            //attron(COLOR_PAIR(1));
	            move(LINES-1, COLS-1);
	           // refresh();
	        }
	    }
	}
	else if (ch == 'q') {
	    endwin();
	    break;
	}
	else {
	    attron(COLOR_PAIR(1));
	    mvprintw(20, 20, "not");
	    attroff(COLOR_PAIR(1));
	    refresh();
	}
    }
}
