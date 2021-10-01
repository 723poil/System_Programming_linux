/*
file : write0.c
author : 2017110051_이상협
datetime : 2021-10-01 10:45
description : thie file is for sending messages to other users, 
              you can send message and when you want to end, input Ctrl+D.
*/

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

int main(int ac, char *av[]) {

    int fd; // file description
    char buf[BUFSIZ]; // the space where data passes

    if(ac != 2) { // check args, if ac != 2, error output and exit
	fprintf(stderr, "usage: write0 ttyname\n");
	exit(1);
    }

    /* open device */
    fd = open(av[1], O_WRONLY);
    
    if(fd == -1) {
	perror(av[1]);
	exit(1);
    }

    while(fgets(buf, BUFSIZ, stdin) != NULL) { // loop send message until EOF on input
	if (write(fd, buf, strlen(buf)) == -1) {
	    break;
	}
    }

    close(fd); // close file
}

