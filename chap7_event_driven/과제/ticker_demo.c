/*
file        : ticker_demo.c
author      : 2017110051_이상협
datetime    : 2021-10-15 16:35
description : execute the countdown to a regular time using the interval timer. 
*/
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#include <stdlib.h>

int main() {

    void countdown(int);        // handler

    signal(SIGALRM, countdown); // register signal

    if(set_ticker(500) == -1) {
	perror("set_ticker");
    }
    else {
	while(1) pause();       // block
    }
}

void countdown(int signum) {
    static int num = 10;
    printf("%d ..", num--);
    fflush(stdout); // outputs at once if not used

    if(num < 0) {   // if num < 0, print "DONE!" and exit
	printf("DONE!\n");
	exit(0);
    }
}
