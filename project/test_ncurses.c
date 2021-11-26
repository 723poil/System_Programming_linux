#include <stdio.h>
#include <ncurses.h>

int main() {

    initscr();
    start_color();
    init_pair(1, COLOR_RED,COLOR_YELLOW);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);

    for(int i = 0; i < 2; i++) {
	move(i, 0);
	attron(COLOR_PAIR(i+1));
	printw("pair : %d color", i+1);
    }

    refresh();
    getch();
    endwin();
    return 0;
}
