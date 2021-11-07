/*
file        : waitdemo3.c
author      : 2017110051 이상협
datetime    : 2021-11-05 20:30
description : process with signal without using the wait function 
             to receive the status of child by handling
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#define DELAY 5

int newpid; // after changing to a global variable, it can also be used by handler

int main() {

    void child_code(), parent_code();

    printf("before: mypid is %d\n", getpid());

    if ( (newpid = fork()) == -1 ) {
	perror("fork");
    }
    else if ( newpid == 0 ) {
	child_code(DELAY);
    }
    else {
	parent_code();
    }

    return 0;
}

void child_code(int delay) {

    printf("child %d here. will sleep for %d seconds\n", getpid(), delay);
    sleep(delay);
    printf("child done. about to exit\n");
    exit(17);
}

void parent_code() {

    void handler();

    signal(SIGCHLD, handler);  // SIGCHLD handling
    while(1) {                 // print out messages every second
	sleep(1);
	printf("parent is busy...\n");
    }
}

void handler(int sig_num) {  
    // output status of child through handler without performing in parent_code()

    int wait_rv;
    int child_status;
    int high_8, low_7, bit_7;

    wait_rv = wait(&child_status);
    printf("done waiting for %d. Wait returned: %d\n", newpid, wait_rv);

    high_8 = child_status >> 8;
    low_7 = child_status & 0x7F;
    bit_7 = child_status & 0x80;
    printf("status: exit=%d, sig=%d, core=%d\n", high_8, low_7, bit_7);

    exit(0);
}
