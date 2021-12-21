#include <stdio.h>
#include <signal.h>
#include <ncurses.h>
#include <termios.h>
#include <pthread.h>
#include <fcntl.h>

#define WIDTH COLS
#define HEIGHT LINES-5

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int startx = 0;
int starty = 5;

int curx = 1;
int cury = 1;

WINDOW *main_win;

void *mouse_event();
void *key_event();
void set_nodelay_mode();
void tty_mode(int );
void color_black();

int main(void) {
   
    tty_mode(0);
    set_nodelay_mode();

    pthread_t mouse_thread;

    pthread_create(&mouse_thread, NULL, mouse_event, (void *)NULL);
    pthread_join(mouse_thread, NULL);

    tty_mode(1);

    return 0;
}

void set_nodelay_mode() {

    int termflags;
    termflags = fcntl(0, F_GETFL);
    termflags |= O_NDELAY;
    fcntl(0, F_SETFL, termflags);
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

void *mouse_event() {

    int ch;

    initscr();
    noecho();
    keypad(stdscr, TRUE);

    main_win = newwin(HEIGHT, WIDTH, starty, startx);
    box(main_win, 0, 0);
    wrefresh(main_win);

    mousemask(BUTTON1_PRESSED, NULL);
    mouseinterval(0);

    color_black();

    int conti = 1;

    while(conti) {
	pthread_t key_t;
        pthread_create(&key_t, NULL, key_event, (void *)NULL);
        pthread_join(key_t, (void *)&conti);
    }
    return NULL;
}

void color_black() {

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);

    attron(COLOR_PAIR(1));
    bkgd(COLOR_PAIR(1));
    attroff(COLOR_PAIR(1));

    wmove(main_win, cury, curx);
    box(main_win, 0, 0);

    wrefresh(main_win);
    refresh();
}

void *key_event() {
    
    int ch = getch();
    MEVENT event;
    int conti = 1;
    
    if(ch == KEY_MOUSE) {
	if(getmouse(&event) == OK) {
	    if(event.bstate & BUTTON1_PRESSED) {
		if(event.y-5 > 0 && event.y-5 < HEIGHT-1 && event.x < WIDTH-1 && event.x > 0) {
	            cury = event.y - 5;
		    curx = event.x;
		    wmove(main_win, cury, curx);
		    box(main_win, 0, 0);
		    wrefresh(main_win);
		}
	    }
	}
    }
    else if (ch == KEY_DOWN) {
	if(cury < HEIGHT-2) {
	    cury = cury + 1;
	}
	else {
	    cury = HEIGHT-2;
	}
	box(main_win, 0, 0);
	wmove(main_win, cury, curx);
	wrefresh(main_win);
    }
    else if (ch == KEY_UP) {
	if(cury > 1) {
	    cury = cury - 1;
	}
	else {
	    cury = 1;
	}
	box(main_win, 0, 0);
	wmove(main_win, cury, curx);
	wrefresh(main_win);
    }
    else if (ch == KEY_RIGHT) {
	if(curx < WIDTH-2) {
	    curx = curx + 1;
	}
	else {
	    curx = WIDTH-2;
	}
	box(main_win, 0, 0);
	wmove(main_win, cury, curx);
	wrefresh(main_win);
    }
    else if (ch == KEY_LEFT) {
	if(curx > 1) {
	    curx = curx-1;
	}
	else {
	    curx = 1;
	}
	box(main_win, 0, 0);
	wmove(main_win, cury, curx);
	wrefresh(main_win);
    }
    else if (ch == 'd') {
	pthread_mutex_lock(&lock);
        start_color();
	init_pair(2, COLOR_BLUE, COLOR_GREEN);
	wattron(main_win, COLOR_PAIR(2));
	waddstr(main_win, " ");
	wattroff(main_win, COLOR_PAIR(2));
	box(main_win, 0, 0);
	wmove(main_win, cury, curx);
	wrefresh(main_win);
	pthread_mutex_unlock(&lock);
    }
    else if (ch == 'q') {

	endwin();
	conti = 0;
	return (void *)conti;
    }
    return (void *)conti;
}