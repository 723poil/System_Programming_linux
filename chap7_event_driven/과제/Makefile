all: ticker_demo myhello

myhello: myhello.c set_ticker.c
	gcc -o myhello myhello.c set_ticker.c -lcurses

ticker_demo : ticker_demo.c set_ticker.c
	gcc -o ticker_demo ticker_demo.c set_ticker.c
