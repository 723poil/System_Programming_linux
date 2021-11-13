/*
file        : varlib.c
author      : 2017110051_이상협
datetime    : 2021-11-12 16:00
description : a simple storage system to store name=value pairs
              with facility to mark items as part of the environment
*/
#include <stdio.h>
#include <stdlib.h>
#include "varlib.h"
#include <string.h>

#define MAXVARS 200 /* a linked list would be niver */

struct var {
    	char *str;  /* name=val string */
	int global;     /*    a boolean    */
};

static struct var tab[MAXVARS];              /* the table */
static char *new_string(char*, char*);       /* private methods */
static struct var *find_item(char*, int);

int VLstore(char* name, char *val) { /* traverse list, if found, replace it, else add at end */

    struct var *itemp;
    char *s;
    int rv = 1;

    if ((itemp=find_item(name, 1)) != NULL && (s=new_string(name,val)) != NULL) { /* find spot to put it and make new string */
	if (itemp->str) free(itemp->str);                                             /* has a val ? -> remove it */
	itemp->str = s;                    
	rv = 0;                                           
    }
    return rv;
}

char *new_string(char* name, char *val) { /* returns new string of form name=value or NULL on error */

    char *retval;

    retval = malloc(strlen(name) + strlen(val) + 2 );
    if ( retval != NULL ) sprintf(retval, "%s=%s", name, val);
    return retval;
}

char *VLlookup(char *name) { /* returns value of var or empty string if not there */

    struct var *itemp;

    if( (itemp = find_item(name, 0)) != NULL ) return itemp->str + 1 + strlen(name);
    return "";
}

int VLexport(char *name) { /* marks a var for export, adds it if not there */

    struct var *itemp;
    int rv = 1;

    if( (itemp = find_item(name, 0)) != NULL ) {
	itemp->global = 1;
	rv = 0;
    }
    else if ( VLstore(name, "") == 1 ) rv = VLexport(name);
    return rv;
}

static struct var * find_item( char *name, int first_blank) { /* searches table for an item */

    int i;
    int len = strlen(name);
    char *s;

    for ( i=0; i<MAXVARS && tab[i].str != NULL ; i++ ) {
	s = tab[i].str;
	if ( strncmp(s, name, len) == 0 && s[len] == '=' ) return &tab[i];
    }

    if(i<MAXVARS && first_blank) return &tab[i];
    return NULL; /* not found */
}

void VLlist() { /* performs the shell`s set command */
    
    int i;
    for (i=0; i<MAXVARS && tab[i].str != NULL; i++) {
	if (tab[i].global) printf("  * %s\n", tab[i].str);
	else printf("    %s\n", tab[i].str);
    }
}

int VLenviron2table(char *env[]) { /* initialize the variable table by loading array of strings */

    int i;
    char *newstring;

    for(i=0; env[i] != NULL; i++ ) {
	if(i == MAXVARS) return 0;
	newstring = malloc(1+strlen(env[i]));
	if(newstring == NULL) return 0;
	strcpy(newstring, env[i]);
	tab[i].str = newstring;
	tab[i].global = 1;
    }

    while(i < MAXVARS) { /* i know we don`t need this */
	tab[i].str = NULL;   /* static globals are nulled */
	tab[i++].global = 0; /* by default                */
    }

    return 1;
}

char ** VLtable2environ() { /* build an array of pointers suitable for making a new environment */

    int i, j, n=0; /* index, another index, counter */
    char **envtab; /* array of porinters            */

    /* first, count the number of global variables */
    for (i = 0; i<MAXVARS && tab[i].str != NULL ; i++) {
	if (tab[i].global == 1) n++;
    }

    /* then, allocate space for that many variables */
    envtab = (char**)malloc((n+1)*sizeof(char*));

    if (envtab == NULL) return NULL;

    /* then, load the array with pointers           */
    for (i=0, j=0; i<MAXVARS && tab[i].str != NULL; i++) {
	if (tab[i].global == 1) envtab[j++] = tab[i].str;
    }

    envtab[j] = NULL;
    return envtab;
}
