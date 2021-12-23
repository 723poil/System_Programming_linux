#include <stdio.h>
#include <signal.h>
#include <ncurses.h>
#include <termios.h>
#include <pthread.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define WIDTH COLS
#define HEIGHT LINES-5
#define FILENUM 10

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

char files[FILENUM][20];
int filenum = 0;

int startx = 0;
int starty = 5;

int curx = 1;
int cury = 1;

int cs = 1;

int thepipe[2];

char file_name[20];
char file_link[25];

WINDOW *main_win;
WINDOW *menu_win;
WINDOW *choice_win;

void *draw_event();
void *key_event();
void set_nodelay_mode();
void tty_mode(int );
void color_black(WINDOW * win);
void QUIT_handler();
int set_ticker(int);
void auto_set();
void choice_file();
void search_file();

int main(void) {
   
    tty_mode(0);
    set_nodelay_mode();
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, QUIT_handler);

    pthread_t mouse_thread;

    choice_file();
	
    pthread_create(&mouse_thread, NULL, draw_event, (void *)NULL);
    pthread_join(mouse_thread, NULL);

    tty_mode(1);

    return 0;
}

void search_file() {

	int pid;

	if (pipe(thepipe) == -1) {
		perror("pipe");
		exit(2);
	}

	if( (pid = fork()) == -1 ) {
		perror("fork");
		exit(2);
	}
	else if ( pid > 0 ) {
		// children
		close(thepipe[0]);
		dup2(thepipe[1], 1);
		close(thepipe[1]);

		execlp("ls", "ls", "draw", NULL);
	}
	else {
		wait(NULL);
	}
}

void choice_file() {

	initscr();
	noecho();
    keypad(stdscr, TRUE);
	clear();

	search_file();

	choice_win = newwin(HEIGHT - 5, 25, 5, 20);
	box(choice_win, 0, 0);
	
    char buffer[BUFSIZ];
	read(thepipe[0], buffer, BUFSIZ);
	
	char *ptr = strtok(buffer, " ");

	while(ptr != NULL) {
		strcpy(files[filenum], ptr);
		//files[filenum] = ptr;
		filenum += 1;
		ptr = strtok(NULL, " ");
	}

	for (int i = 1; i <= filenum; i++) {
        wmove(choice_win, i, 1);
		waddstr(choice_win, files[i-1]);
		box(choice_win, 0, 0);
		wrefresh(choice_win);
	}
	refresh();
	wgetch(choice_win);
	
    mousemask(BUTTON1_PRESSED, NULL);
    mouseinterval(0);

	while(1) {
		wmove(choice_win, 1, 1);
		int ch = getch();
        MEVENT event;

		if(ch == KEY_MOUSE) {
			if(getmouse(&event) == OK) {
				if(event.bstate & BUTTON1_PRESSED) {
					// 좌표 넣기 성공하면 break 넣기
					// 좌표에 맞는 파일 선택해서 변수에 저장하기
					if (event.y > 5 & event.y < HEIGHT & event.x > 20 & event.x < 45) {
						strcpy(file_name, files[event.y - 6]);
						//file_name = files[event.y-6];
						snprintf(file_link, strlen(file_name) + 5, "draw/%s", file_name);
						endwin();
						return ;
					}
				}
			}
		}
		else {
			waddstr(choice_win, "1");
		}
	}
}

void QUIT_handler() {

	remove(file_link);
    
	FILE *f;
    f = fopen(file_link, "a");
    
	putwin(main_win, f);
    fclose(f);
    
    tty_mode(1);
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    
    endwin();
    exit(1);
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

void menu_event() {
	menu_win = newwin(5, WIDTH, 0, 0);
	box(menu_win, 0, 0);
	color_black(menu_win);
	wrefresh(menu_win);

    char output_file[50];
	snprintf(output_file, strlen(file_name) + 12, "FILE NAME - %s", file_name);
	wmove(menu_win, 1, 3);
	waddstr(menu_win, output_file);
	wmove(menu_win, 1, 28);
	waddstr(menu_win, "F1 - ");
	wattron(menu_win, COLOR_PAIR(1));
	waddstr(menu_win, " ");
	wattroff(menu_win, COLOR_PAIR(1));
	wmove(menu_win, 1, 36);
	waddstr(menu_win, "F2 - ");
	wattron(menu_win, COLOR_PAIR(2));
	waddstr(menu_win, " ");
	wattroff(menu_win, COLOR_PAIR(2));
	wmove(menu_win, 1, 44);
	waddstr(menu_win, "F3 - ");
	wattron(menu_win, COLOR_PAIR(3));
	waddstr(menu_win, " ");
	wattroff(menu_win, COLOR_PAIR(3));
	wmove(menu_win, 1, 52);
	waddstr(menu_win, "F4 - ");
	wattron(menu_win, COLOR_PAIR(4));
	waddstr(menu_win, " ");
	wattroff(menu_win, COLOR_PAIR(4));
	wmove(menu_win, 2, 28);
	waddstr(menu_win, "F5 - ");
	wattron(menu_win, COLOR_PAIR(5));
	waddstr(menu_win, " ");
	wattroff(menu_win, COLOR_PAIR(5));
	wmove(menu_win, 2, 36);
	waddstr(menu_win, "F6 - ");
	wattron(menu_win, COLOR_PAIR(6));
	waddstr(menu_win, " ");
	wattroff(menu_win, COLOR_PAIR(6));
	wmove(menu_win, 2, 44);
	waddstr(menu_win, "F7 - ");
	wattron(menu_win, COLOR_PAIR(7));
	waddstr(menu_win, " ");
	wattroff(menu_win, COLOR_PAIR(7));
	wmove(menu_win, 2, 52);
	waddstr(menu_win, "F8 - ");
	wattron(menu_win, COLOR_PAIR(8));
	waddstr(menu_win, " ");
	wattroff(menu_win, COLOR_PAIR(8));

	wmove(menu_win, 1, 60);
	waddstr(menu_win, "d - draw | Ctrl+\\ - save and exit | q - not save and exit");

	wrefresh(menu_win);
}

void color_setting() {

	start_color();
	init_pair(0, COLOR_BLACK, COLOR_BLACK);
	init_pair(1, COLOR_BLACK, COLOR_WHITE);
	init_pair(2, COLOR_WHITE, COLOR_BLACK);
	init_pair(3, COLOR_BLUE, COLOR_RED);
	init_pair(4, COLOR_YELLOW, COLOR_GREEN);
	init_pair(5, COLOR_GREEN, COLOR_YELLOW);
	init_pair(6, COLOR_RED, COLOR_BLUE);
	init_pair(7, COLOR_CYAN, COLOR_MAGENTA);
	init_pair(8, COLOR_MAGENTA, COLOR_CYAN);
}

void *draw_event() {

	void menu_event();

    initscr();
    noecho();
    keypad(stdscr, TRUE);
	color_setting();

	set_ticker(600000);

	signal( SIGALRM, auto_set);

    //main_win = newwin(HEIGHT, WIDTH, starty, startx);
	FILE *w;
	w = fopen(file_link, "r");
	main_win = getwin(w);
	fclose(w);
    box(main_win, 0, 0);
    wrefresh(main_win);

    menu_event();

    mousemask(BUTTON1_PRESSED, NULL);
    mouseinterval(0);

    color_black(main_win);
    color_black(menu_win);
   
    refresh(); 
    box(menu_win, 0, 0);
    wrefresh(menu_win);
    wmove(main_win, cury, curx);
    wrefresh(main_win);

    int conti = 1;

    while(conti) {
		pthread_t key_t;
        pthread_create(&key_t, NULL, key_event, (void *)NULL);
        pthread_join(key_t, (void *)&conti);
    }
    return NULL;
}

void auto_set() {

    int pid;

	if( (pid = fork()) == -1) {
		perror("fork");
		exit(1);
	}
	else if ( pid > 0 ) {

		remove(file_link);

		FILE *f;
		f = fopen(file_link, "a");
		
		putwin(main_win, f);
		fclose(f);
		exit(1);
	}
}

void color_black(WINDOW* win) {

    pthread_mutex_lock(&lock);
    
    cs = 0;

    bkgd(COLOR_PAIR(cs));

    cs = 1;

    box(win, 0, 0);

    wrefresh(win);

    pthread_mutex_unlock(&lock);
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
		wattron(main_win, COLOR_PAIR(cs));
		waddstr(main_win, " ");
		wattroff(main_win, COLOR_PAIR(cs));
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
	else if (ch == KEY_F(1)) {
		cs = 1;
	}
	else if (ch == KEY_F(2)) {
		cs = 2;
	}
	else if (ch == KEY_F(3)) {
		cs = 3;
	}
	else if (ch == KEY_F(4)) {
		cs = 4;
	}
	else if (ch == KEY_F(5)) {
		cs = 5;
	}
	else if (ch == KEY_F(6)) {
		cs = 6;
	}
	else if (ch == KEY_F(7)) {
		cs = 7;
	}
	else if (ch == KEY_F(8)) {
		cs = 8;
	}
    return (void *)conti;
}
