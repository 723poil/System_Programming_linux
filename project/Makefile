all : test_ncurses main

test_ncurses : test_ncurses.c
	gcc -o test_ncurses test_ncurses.c -lcurses

main : main.c
	gcc -o main main.c set_ticker.c -lcurses -lpthread
