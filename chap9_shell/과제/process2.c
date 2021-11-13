/*
file        : process2.c
author      : 2017110051_이상협
datetime    : 2021-11-12 16:00
description : process user command
              
*/
#include <stdio.h>
#include "smsh.h"
#include "varlib.h"

int is_control_command(char *);
int do_control_command(char **);
int ok_to_execute();

int process(char **args) {

    int rv = 0;

    if ( args[0] == NULL ) rv = 0;
    else if ( is_control_command(args[0]) ) rv = do_control_command(args); /* if control command, do control command */
    else if ( ok_to_execute() ) {                                          /* execute command */
	if ( !builtin_command(args, &rv) ) rv = execute(args);
    }

    return rv;
}
