/*
file        : splitline.c
author      : 2017110051_이상협
datetime    : 2021-11-12 15:30
description : command reading and parsing functions for smsh
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "smsh.h"

char *next_cmd(char *prompt, FILE *fp) { /* read next command line from fp */
        
    char *buf;              /*    the buffer    */
    int bufspace = 0;       /*    total size    */
    int pos = 0;            /* current position */
    int c;                  /*    input char    */

    printf("%s", prompt);                               /* print prompt  */
    while ( (c = getc(fp)) != EOF) {
	if (pos+1 >= bufspace){                             /*  1 for '\0'   */
	    if (bufspace == 0) buf = emalloc(BUFSIZ);       /*  start first  */
	    else buf = erealloc(buf, bufspace+BUFSIZ);      /* expand memory */
	    bufspace += BUFSIZ;                             /*  update size  */
	}

	if (c == '\n') break;

	buf[pos++] = c;
    }

    if (c == EOF && pos == 0) return NULL;     /* EOF and no input */

    buf[pos] = '\0';
    return buf;
}

#define is_delim(x) ((x) == ' ' || (x) == '\t')

char ** splitline(char *line) { /* parse a line an array of strings */

    char *newstr();
    char **args;
    int spots = 0;        /* spots in table */
    int bufspace = 0;     /* bytes in table */
    int argnum = 0;       /*   slots used   */
    char *cp = line;      /* pos in string  */
    char *start;
    int len;

    if (line == NULL) return NULL;   /* handle spectial case */

    args = emalloc(BUFSIZ);          /*   initialize array   */
    bufspace = BUFSIZ;
    spots = BUFSIZ/sizeof(char *);

    while(*cp != '\0') {
	    while ( is_delim(*cp) ) cp++;    /* skip leading spaces  */

	    if (*cp == '\0') break;          /* quit at end-o-string */

	    if (argnum+1 >= spots) {
	        args = erealloc(args, bufspace+BUFSIZ);
	        bufspace += BUFSIZ;
	        spots += (BUFSIZ/sizeof(char *));
	    }

        /* mark start, find end of word */
	    start = cp;
	    len = 1;
	    while (*++cp != '\0' && !(is_delim(*cp))) len++;

	    args[argnum++] = newstr(start, len);
    }
    args[argnum] = NULL;
    return args;
}

char *newstr(char *s, int l) { /* make string */

    char *rv = emalloc(l+1);

    rv[l] = '\0';
    strncpy(rv, s, l);
    return rv;
}

void freelist(char **list) { /* free memory */

    char **cp = list;
    while(*cp) {
	 free(*cp++);
    }
    free(list);
}

void *emalloc(size_t n) {  /* malloc */

    void *rv;
    if ( (rv = malloc(n)) == NULL) fatal("out of meomory", "", 1);
    return rv;
}

void *erealloc(void *p, size_t n) { /* realloc */

    void *rv;
    if ( (rv = realloc(p, n)) == NULL ) fatal("realloc() failed", "", 1);
    return rv;
}

