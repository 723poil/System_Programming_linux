/*
file        : pipe2.c
author      : 2017110051_이상협
datetime    : 2021-11-19 13:00
description : demonstrates how to create a pipeline from one process to another
              takes three args, each a command, and connects
			  av[1]`s output to input of av[2]
			  av[2]`s output to input of av[3]
			  effect : command1 | command2 | command3
*/
#include <stdio.h>
#include <unistd.h>

#define oops(m,x) { perror(m); exit(x); }

int main(int ac, char **av) {

	/* two pipe (command1 - command2, command2 - command3) */
    int thepipe[2], thenextpipe[2], newpid, pid;
    
    if( ac != 4 ) { /* number of av */
	fprintf(stderr, "usage: pipe cmd1 cmd2\n");
	exit(1);
    }
    if (pipe(thepipe) == -1) oops("Cannot get a pipe", 1); /* get pipe (command1 - command2) */

    if ( (pid = fork()) == -1) oops("Cannot fork", 2);  /* get process */

    if (pid > 0) {  /* parent */
		close(thepipe[1]);  /* parent doesn`t write to pipe */

		if(dup2(thepipe[0], 0) == -1)
	    	oops("could not redirect stdin", 3);

		close(thepipe[0]);  /* stdin is duped, close pipe */
	
		if(pipe(thenextpipe) == -1) oops("Cannot get a nextpipe", 6);

		if( (newpid = fork()) == -1) oops("Cannot next fork", 7);

		if (newpid > 0 ) {  /* parent-parent */
	 	   close(thenextpipe[1]); /* parent-parent doesn`t read from pipe */

	 	   if(dup2(thenextpipe[0], 0) == -1)
			oops("could not redirect stdin", 8);
	    
		    close(thenextpipe[0]); /* stdin is duped, close pipe */

		    execlp(av[3], av[3], NULL);
		    oops(av[3], 9);
		}
							/* parent-child */

		close(thenextpipe[0]);   /* parent-child doesn`t read form pipe */

		if(dup2(thenextpipe[1], 1) == -1)
	    	oops("could not redirect stdout", 10);
	
		close(thenextpipe[1]);   /* stdout is duped, close pipe */

		execlp(av[2], av[2], NULL);
		oops(av[2], 4);
    }
    else {  /* child */

        close(thepipe[0]);   /* child doesn`t read form pipe */

        if(dup2(thepipe[1], 1) == -1)
	    oops("could not redirect stdout", 4);

        close(thepipe[1]);   /* stdout is duped, close pipe */
        execlp(av[1], av[1], NULL);
        oops(av[1], 5);
    }

    return 0;
}
