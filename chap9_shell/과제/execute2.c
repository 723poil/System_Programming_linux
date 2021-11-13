/*
file        : execute2.c
author      : 2017110051_이상협
datetime    : 2021-11-12 16:00
description : code used by small-shell to execute commands
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "varlib.h"
#include "smsh.h"

int execute(char *argv[]) { /* run a program passing it arguments */

    int pid;
    int child_info = -1;

    if ( argv[0] == NULL ) return 0;  /* notring succeeds */

    if ( (pid = fork()) == -1 ) {
	perror("fork");
    }
    else if (pid == 0) {              /* child process  */
	environ = VLtable2environ();
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	execvp(argv[0], argv);            /* execute command */
	perror("cannot execute command"); /* if execvp carried out normally, don`t perform */
	exit(1);
    }
    else {                            /* parent process */
	if ( wait(&child_info) == -1 ) {
	    perror("wait");
	}
    }
    return child_info;                /* send child_info status */
}
