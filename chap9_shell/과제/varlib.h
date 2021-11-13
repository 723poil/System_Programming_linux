/*
file        : varlib.h
author      : 2017110051_이상협
datetime    : 2021-11-12 16:00
description : header file for varlib
*/
int builtin_command(char **, int *);
int VLstore(char *, char *);
char* VLlookup(char *);
int VLexport(char *);
void VLlist();
int VLenviron2table(char**);
char** VLtable2environ();
