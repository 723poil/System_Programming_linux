/*
file        : pipe3.c
author      : 2017110051_이상협
datetime    : 2021-11-19 15:30
description : demonstrates how to create a pipeline from one process to another
              takes three args, each a command, and connects and attach
			  av[1]`s output to input of av[2]
			  av[2]`s output to input of av[3]
			  av[3]`s output to input of av[4]
			  effect : command1 | command2 | command3 > filename
*/
#include <stdio.h>
#include <unistd.h>

#define oops(m,x) { perror(m); exit(x); }

int main(int ac, char **av) {

    int thepipe[2], thenextpipe[2], newpid, pid;

    int fd;

    if( ac != 5 ) {
		fprintf(stderr, "usage: pipe cmd1 cmd2\n");
		exit(1);
    }
    if (pipe(thepipe) == -1) oops("Cannot get a pipe", 1);

    if ( (pid = fork()) == -1) oops("Cannot fork", 2);

    if (pid > 0) { /* parent */
		close(thepipe[1]);

		if(dup2(thepipe[0], 0) == -1)
		    oops("could not redirect stdin", 3);

		close(thepipe[0]);
	
		if(pipe(thenextpipe) == -1) oops("Cannot get a nextpipe", 6);

		if( (newpid = fork()) == -1) oops("Cannot next fork", 7);

		if (newpid > 0 ) { /* parent-parent */
		    close(thenextpipe[1]);

		    if(dup2(thenextpipe[0], 0) == -1)
			oops("could not redirect stdin", 8);
	    
		    close(thenextpipe[0]);
	    
	    
		    if(pipe(thepipe) == -1) oops("Cannot get a pipe", 1);

		    if( (pid = fork()) == -1) oops("Cannot fork", 2);

		    if (pid > 0) { /* parent-parent-parent : attach to the file */
				close(thepipe[1]);

				if(dup2(thepipe[0], 0) == -1)
				    oops("could not redirect stdin", 8);

				close(thepipe[0]);

				close(1);
				fd = creat(av[4], 0644);
		
				char buf[BUFSIZ];
				int len;

				while((len = read(0, buf, BUFSIZ))){ /* store input data to file */
				    if( write(1, buf, len) != len){
						oops("Read error", 1);
				    }
				}
	    	}
	    	else{    /* parent-parent-child : command3 */
	    		close(thepipe[0]);

				if (dup2(thepipe[1], 1) == -1)
				    oops("could not redirect stdout", 10);

				close(thepipe[1]);

	    		execlp(av[3], av[3], NULL);
	    		oops(av[3], 9);
	    	}
		}
		else {  /* parent - child : command2 */
		    close(thenextpipe[0]);

		    if(dup2(thenextpipe[1], 1) == -1)
		        oops("could not redirect stdout", 10);
	
		    close(thenextpipe[1]);

		    execlp(av[2], av[2], NULL);
		    oops(av[2], 4);
		}
    }
    else { /* child : command1*/

        close(thepipe[0]);

        if(dup2(thepipe[1], 1) == -1)
		    oops("could not redirect stdout", 4);

        close(thepipe[1]);
        execlp(av[1], av[1], NULL);
        oops(av[1], 5);
    }

    return 0;
}
