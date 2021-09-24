/*
file : spwd.c
author : 2017110051_이상협
datetime : 2021-09-24 12:02
description : It is a file that outputs a path,
              and it outputs to the root using recursive.
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

ino_t get_inode(char *);
void printpathto(ino_t);
void inum_to_name(ino_t, char*, int);

int main() {
    printpathto(get_inode(".")); // print path to here
    putchar('\n');               // then add newline
    return 0;
}

void printpathto(ino_t this_inode) { // print path leading down to an object with this inode
    ino_t my_inode;                  // using recursive
    char its_name[BUFSIZ];

    if(get_inode("..") != this_inode) {
	chdir("..");                                  // up one dir
	inum_to_name(this_inode, its_name, BUFSIZ);   // get its name
	my_inode = get_inode(".");                    // print head
	printpathto(my_inode);                        // recursively
	printf("/%s", its_name);                      // name of this
    }
}

void inum_to_name(ino_t inode_to_find, char *namebuf, int buflen) { // looks through current directory for a file
                                                                    // with this inode number and copies its name into namebuf
    DIR *dir_ptr;                          // the directory
    struct dirent *direntp;                // each entry
    dir_ptr = opendir(".");
    
    if(dir_ptr == NULL) {
	perror(".");
	exit(1);
    }

    while((direntp = readdir(dir_ptr)) != NULL) {
	if(direntp->d_ino == inode_to_find) {
	    strncpy(namebuf, direntp->d_name, buflen);
	    namebuf[buflen-1] = '\0'; // just in case
	    return;
	}
    }

    fprintf(stderr, "error looking for inum %d\n", inode_to_find);
    exit(1);
}

ino_t get_inode(char *fname) { // using fname, return fname`s inode
    struct stat info;

    if(stat(fname, &info) == -1) {
	fprintf(stderr, "Cannot stat ");
	perror(fname);
	exit(1);
    }
    return info.st_ino;
}
