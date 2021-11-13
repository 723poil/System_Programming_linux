/*
file        : smsh.h
author      : 2017110051_이상협
datetime    : 2021-11-12 15:00
description : header file for about smsh
*/
#define YES 1
#define NO 0

extern char **environ;

char *next_cmd();
char **splitline(char *);
void freelist(char **);
void *emalloc(size_t);
void *erealloc(void *, size_t);
int execute(char **);
void fatal(char *, char *, int);
int process(char **);
