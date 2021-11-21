/*
file        : read.c
author      : 2017110051_이상협
datetime    : 2021-11-19 11:50
description : 
*/
#include <stdio.h>
#include <unistd.h>

#define oops(m,x) { perror(m); exit(x); }

int main(int ac, char **av) {

    int thepipe[2], newfd, pid;

    int len;
    char buf[BUFSIZ];
    int read_len;

    if( ac != 3 ) {
	fprintf(stderr, "usage: pipe cmd1 cmd2\n");
	exit(1);
    }
    if (pipe(thepipe) == -1) oops("Cannot get a pipe", 1);

    if ( (pid = fork()) == -1) oops("Cannot fork", 2);

    if (pid > 0) {
	close(thepipe[1]);

	if(dup2(thepipe[0], 0) == -1)
	    oops("could not redirect stdin", 3);

	close(thepipe[0]);
//	wait(NULL);

	execlp(av[1], av[1], NULL);
	oops(av[1], 4);
    }

    close(thepipe[0]);

    if(dup2(thepipe[1], 1) == -1)
	oops("could not redirect stdout", 4);

    close(thepipe[1]);
    char* arglist[] = {"cat", av[2], NULL};
    execvp(arglist[0], arglist);
    
    oops(av[2], 5);

    return 0;
}
