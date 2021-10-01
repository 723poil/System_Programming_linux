/*
file        : mywrite.c
author      : 2017110051_이상협
datetime    : 2021-10-01 12:00
description : thie file is for sending messages to other users, 
              and when you start sending messages, 
	      you start by sending your ID and the time you start sending them. 
	      at the end of the message transmission, print "EOF" and end it.
*/

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>    // to get user_id
#include <sys/types.h> // to get user_id
#include <pwd.h>       // to get user_id (struct passwd) 
#include <time.h>      // to get current time

struct passwd* get_user_id(); // for to get user_id
struct tm* get_time_now();    // for to get current time

int main(int ac, char *av[]) {

    int fd;
    char buf[BUFSIZ];
    struct passwd *pwd; // structure variable for storing user_id
    struct tm *ct;      // structure variable for storing current time

    if(ac != 2) {
	fprintf(stderr, "usage: write0 ttyname\n");
	exit(1);
    }

    fd = open(av[1], O_WRONLY);
    
    if(fd == -1) {
	perror(av[1]);
	exit(1);
    }

    pwd = get_user_id(); // calling function to get user info
    ct = get_time_now(); // calling function to get time info

    /* print out who sent the message and when  */
    printf("Message from %s at %02d:%02d ...\n", pwd->pw_name, ct->tm_hour, ct->tm_min);

    while(fgets(buf, BUFSIZ, stdin) != NULL) {
	if (write(fd, buf, strlen(buf)) == -1) {
	    break;
	}
    }

    printf("EOF\n"); // if input ctrl+D, print out "EOF"

    close(fd);

    return 0;
}

struct passwd* get_user_id() {

    return getpwuid(geteuid()); // get user_id
}

struct tm* get_time_now() {

    time_t T = time(NULL);  // set the value to the current time
    return localtime(&T);   // get the current time
}
