/*
file        : pipe2.c
author      : 2017110051_이상협
datetime    : 2021-11-19 13:00
description : 
*/
#include <stdio.h>
#include <unistd.h>

#define oops(m,x) { perror(m); exit(x); }

int main(int ac, char **av) {

    int thepipe[2], thenextpipe[2], newpid, pid;

    if( ac != 4 ) {
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
	
	if(pipe(thenextpipe) == -1) oops("Cannot get a nextpipe", 6);

	if( (newpid = fork()) == -1) oops("Cannot next fork", 7);

	if (newpid > 0 ) {
	    close(thenextpipe[1]);

	    if(dup2(thenextpipe[0], 0) == -1)
		oops("could not redirect stdin", 8);
	    
	    close(thenextpipe[0]);
//	    wait(NULL);

	    execlp(av[3], av[3], NULL);
	    oops(av[3], 9);
	}

	close(thenextpipe[0]);

	if(dup2(thenextpipe[1], 1) == -1)
	    oops("could not redirect stdout", 10);
	
	close(thenextpipe[1]);

	execlp(av[2], av[2], NULL);
	oops(av[2], 4);
    }
    else {

        close(thepipe[0]);

        if(dup2(thepipe[1], 1) == -1)
	    oops("could not redirect stdout", 4);

        close(thepipe[1]);
        execlp(av[1], av[1], NULL);
        oops(av[1], 5);
    }

    return 0;
}
