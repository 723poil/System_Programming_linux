/*
file        : controlflow.c
author      : 2017110051_이상협
datetime    : 2021-11-12 16:00
description : in the case of a general command, it has a function to discriminate
              and execute the control command.
*/
#include <string.h>
#include <stdio.h>
#include "smsh.h"

enum states { NEUTRAL, WANT_THEN, THEN_BLOCK };
enum results { SUCCESS, FAIL };

static int if_state = NEUTRAL;
static int if_result = SUCCESS;
static int last_stat = 0;

int syn_err(char *);

int ok_to_execute() { /* determine the shell should execute a command */

    int rv = 1;                            /* default is positive */

    if ( if_state == WANT_THEN ) {
	syn_err("then expected");
	rv = 0;
    }
    else if ( if_state == THEN_BLOCK && if_result == SUCCESS ) rv = 1;
    else if ( if_state == THEN_BLOCK && if_result == FAIL ) rv = 0;
    return rv;
}

int is_control_command(char *s) { /* boolean to report if the command is a shell control command */

    return (strcmp(s, "if")==0 || strcmp(s, "then")==0 || strcmp(s, "fi")==0);  /* return 0 or 1 */
}

int do_control_command(char **args) { /* process "if", "thne", "fi" - change state or detect error */

    char *cmd = args[0];
    int rv = -1;

    if (strcmp(cmd, "if")==0) {                                  /*     if command     */
	if ( if_state != NEUTRAL ) rv = syn_err("if unexpected");
    	else {
		last_stat = process(args+1);                             /*     if "------"    */
		if_result = (last_stat == 0 ? SUCCESS : FAIL );
		if_state = WANT_THEN;                                    /* state - WANT_THEN  */
		rv = 0;
    	}
    }
    else if (strcmp(cmd, "then")==0) {                           /*    then command    */
	if( if_state != WANT_THEN ) rv = syn_err("then unexpected");
	else {
	    if_state = THEN_BLOCK;                                   /* state - THEN_BLOCK */
	    rv = 0;
	}
    }
    else if ( strcmp(cmd, "fi")==0 ) {                           /*     fi command     */
	if ( if_state != THEN_BLOCK ) rv = syn_err("fi unexpected");
	else {
	    if_state = NEUTRAL;                                      /*  state - NEUTRAL   */
	    rv = 0;
	}
    }
    else fatal("internal error processing:", cmd, 2);

    return rv;
}

int syn_err(char *msg) { /* handles syntax errors in control structures */

    if_state = NEUTRAL;
    fprintf(stderr, "syntax error: %s\n", msg);
    return -1;
}
