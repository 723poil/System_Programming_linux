/*
file        : builtin.c
author      : 2017110051_이상협
datetime    : 2021-11-12 16:00
description : builtin.c contains the switch and the functions for builtin commands
*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "smsh.h"
#include "varlib.h"

int assign(char *);
int okname(char *);

int builtin_command(char **args, int *resultp) { /* run a builtin command */

    int rv = 0;

    if (strcmp(args[0], "set") == 0 ) {          /* 'set' command ? */
	VLlist();
	*resultp = 0;
	rv = 1;
    }
    else if (strchr(args[0], '=') != NULL ) {    /* assignment cmd  */
	*resultp = assign(args[0]);
	if ( *resultp != -1 ) rv = 1;
    }
    else if (strcmp(args[0], "export") == 0 ) {  /*   if 'export    */
	if ( args[1] != NULL && okname(args[1]) ) *resultp = VLexport(args[1]);
	else *resultp = 1;

	rv = 1;
    }
    return rv;
}

int assign(char *str) { /* execute name=val AND ensure that name is legal */

    char *cp;
    int rv;
    
    cp = strchr(str, '=');
    *cp = '\0';
    rv = ( okname(str) ? VLstore(str, cp+1) : -1 );
    *cp = '=';
    return rv;
}

int okname(char *str) { /* determines if a string is a legal variable name */

    char *cp;

    for(cp = str; *cp; cp++ ) {
	if ((isdigit(*cp) && cp==str) || !(isalnum(*cp) || *cp=='_')) return 0;
    }
    return (cp != str); /* no empty strings, either */
}
