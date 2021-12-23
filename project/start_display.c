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

    void *first_key_event();

	search_file();

	choice_win = newwin(HEIGHT - 5, 25, 5, 20);
	//box(choice_win, 0, 0);
    refresh();
    wrefresh(choice_win);

	
    char buffer[BUFSIZ];
	read(thepipe[0], buffer, BUFSIZ);

    // mvwprintw(choice_win, 1, 5, buffer);

	char *ptr = strtok(buffer, " ");

	while(ptr != NULL) {
		strcpy(files[filenum], ptr); 
		files[filenum][strlen(ptr)] = '\0';
		filenum += 1;
        mvwprintw(choice_win, filenum, 5, files[filenum-1]);
		ptr = strtok(NULL, " ");
	}
    wrefresh(choice_win);

	wmove(choice_win, 1, 1);
	//box(choice_win, 0, 0);
	wrefresh(choice_win);
	
    mousemask(BUTTON1_PRESSED, NULL);
    mouseinterval(0);

    int bol = 1;
	while(bol) {
		pthread_t t;
		pthread_create(&t, NULL, first_key_event, &bol);
		pthread_join(t, NULL);
        // mvwprintw(choice_win, 12, 23, bol);
        // wrefresh(choice_win);
	}
    return NULL;
}

void *first_key_event(int *bol) {
	int ch = getch();
        MEVENT event;

		if(ch == KEY_MOUSE) {
			if(getmouse(&event) == OK) {
				if(event.bstate & BUTTON1_PRESSED) {
					// 좌표 넣기 성공하면 break 넣기
					// 좌표에 맞는 파일 선택해서 변수에 저장하기
					if (event.y > 5 && event.y < HEIGHT && event.x > 20 && event.x < 45) {
						strcpy(file_name, files[event.y - 6]);
						//file_name = files[event.y-6];
						snprintf(file_link, strlen(file_name) + 5, "draw/%s", file_name);
						clear();
						endwin();
						*bol = 0;
                        return NULL;
					}
				}
			}
		}
        else if (ch == 'q') {
            exit(1);
        }
}