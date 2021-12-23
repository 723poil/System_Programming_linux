all : test_ncurses main drawing_board

test_ncurses : test_ncurses.c
	gcc -o test_ncurses test_ncurses.c -lcurses

main : main.c
	gcc -o main main.c set_ticker.c -lcurses -lpthread

drawing_board : start_display.c
	gcc -o drawing_board start_display.c -lcurses -lpthread