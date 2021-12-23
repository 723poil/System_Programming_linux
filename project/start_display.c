#include <stdio.h>
#include <signal.h>
#include <ncurses.h>
#include <termios.h>
#include <pthread.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define WIDTH COLS
#define HEIGHT LINES-5
#define FILENUM 10

char files[FILENUM][20];
int filenum = 0;

int thepipe[2];

char file_name[20];
char file_link[25];

WINDOW *choice_win;

void choice_file();
void search_file();
void *first_key_event();

int main(void) {
   
    //tty_mode(0);
    //set_nodelay_mode();
    signal(SIGINT, SIG_IGN);
    //signal(SIGQUIT, QUIT_handler);

    pthread_t mouse_thread;

    choice_file();

    //tty_mode(1);

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

	search_file();

    mousemask(BUTTON1_PRESSED, NULL);
    mouseinterval(0);

	choice_win = newwin(HEIGHT - 5, 25, 5, 20);
	box(choice_win, 0, 0);
    refresh();
    wrefresh(choice_win);

	
    char buffer[BUFSIZ];
	read(thepipe[0], buffer, BUFSIZ);

	char *ptr = strtok(buffer, "\n");

	while(ptr != NULL) {
		strcpy(files[filenum], ptr); 
		files[filenum][strlen(ptr)] = '\0';
		filenum += 1;
        mvwprintw(choice_win, filenum, 1, files[filenum-1]);
        ptr = strtok(NULL, "\n");
	}
    wrefresh(choice_win);

	wmove(choice_win, 1, 1);
	box(choice_win, 0, 0);
	wrefresh(choice_win);

    int start_bool = 1;

	while(start_bool) {
		pthread_t t;
		pthread_create(&t, NULL, first_key_event, (void *) NULL);
		pthread_join(t, (void *)&start_bool);
        mvwprintw(choice_win, 12, 23, start_bool);
        wrefresh(choice_win);
	}
}

void *first_key_event() {
	int ch = getch();
    MEVENT event;

    int conti = 1;

	if(ch == KEY_MOUSE) {
		if(getmouse(&event) == OK) {
			if(event.bstate & BUTTON1_PRESSED) {
				if (event.y > 5 && event.y < HEIGHT && event.x > 20 && event.x < 45) {
					strcpy(file_name, files[event.y - 6]);
					snprintf(file_link, strlen(file_name) + 5, "draw/%s", file_name);
					clear();
					endwin();
                    conti = 0;
                    return (void *)conti;
				}
			}
		}
	}
    else if (ch == 'q') {
        endwin();
        conti = 0;
        return (void *)conti;
    }
    wrefresh(choice_win);
    return (void *)conti;
}