/*
file        : waitdemo2.c
author      : 2017110051 이상협
datetime    : 2021-11-05 20:00
description : create a child process through the fork operation, write the parent process and
             child process to perform different codes, and check if it works normally.
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define DELAY 5       // 5 seconds delay

int main() {

    int newpid;
    void child_code(), parent_code();

    printf("before: mypid is %d\n", getpid());  // output parent process

    if ( (newpid = fork()) == -1 ) {            // create child process
	perror("fork");
    }
    else if ( newpid == 0 ) {                   // perform child process
	child_code(DELAY);
    }
    else {                                      // perform parent process
	parent_code(newpid);
    }

    return 0;
}

void child_code(int delay) {

    printf("child %d here. will sleep for %d seconds\n", getpid(), delay);
    sleep(delay);  // sleep 5 seconds delay
    printf("child done. about to exit\n");
    exit(17);      // send the data to parent process
}

void parent_code(int childpid) {

    int wait_rv;
    int child_status;
    int high_8, low_7, bit_7;

    wait_rv = wait(&child_status);  // wait until child process end
    printf("done waiting for %d. wait returned: %d\n", childpid, wait_rv);

    high_8 = child_status >> 8;     // 1111 1111 0000 0000
    low_7 = child_status & 0x7F;    // 0000 0000 0111 1111
    bit_7 = child_status & 0x80;    // 0000 0000 1000 0000
    printf("status: exit=%d, sig=%d, core=%d\n", high_8, low_7, bit_7);
}
