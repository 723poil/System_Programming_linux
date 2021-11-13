/*
file        : smsh4.c
author      : 2017110051_이상협
datetime    : 2021-11-12 16:30
description : small-shell version 4
              a code that executes processes command by executing a small shell. 
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "smsh.h"
#include "varlib.h"

#define DFL_PROMPT "--_--_--_--> " /* start command line with prompt */

int main() {
    
    char *cmdline, *prompt, **arglist;
    int result, process(char **);
    void setup();

    prompt = DFL_PROMPT;
    setup();

    while ( (cmdline = next_cmd(prompt, stdin)) != NULL ){ /*  input string   */
	if ( (arglist = splitline(cmdline)) != NULL ){         /* separate string */
	    result = process(arglist);                         /* execute command */
	    freelist(arglist);                                 /*   free memory   */
	}
	free(cmdline);
    }
    return 0;
}

void setup() { /* setup signal - ignore and initialize variable table */

    extern char **environ;
    VLenviron2table(environ);
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
}

void fatal(char *s1, char *s2, int n) { /* error output */

    fprintf(stderr, "Error: %s, %s\n", s1, s2);
    exit(n);
}
