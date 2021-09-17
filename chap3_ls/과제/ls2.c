/*
file        : ls2.c
author      : 201711005_SangHyup Lee
datetime    : 2021-09-17 12:00
description : to view a list of files in a directory
	      first - received the directory path as a command factor
	      second - use the path to retrieve and list the file list one by one
*/

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>

// declare function
void do_ls(char []);
void dostat(char *);
void show_file_info(char *, struct stat *);
void mode_to_letters(int, char []);
char *uid_to_name(uid_t);
char *gid_to_name(gid_t);

int main(int ac, char *av[]) {
    
    if (ac == 1) { // if ac == 1, then searching the current route search 
	do_ls(".");
    }
    else {
	while( --ac ) {
	    printf("%s:\n", *++av);
	    do_ls(*av);
	}
    }

    return 0;
}

void do_ls(char dirname[]) { // a function that searches for files in that path
	DIR *dir_ptr;              // the directory
	struct dirent *direntp;    // each entry

	if( (dir_ptr = opendir(dirname)) == NULL ) { // declare the given path to dir_ptr
	    fprintf(stderr, "ls1: cannot open %s\n", dirname);
	}
	else{
	    while( (direntp = readdir(dir_ptr)) != NULL ) { // search for the path files one by one
		dostat(direntp->d_name);
	    }
	    closedir(dir_ptr);
	}
}

void dostat(char *filename) { // a function of reading information on a file
    struct stat info;

    if (stat(filename, &info) == -1) {
	perror(filename);
    }
    else {
	show_file_info(filename, &info);
    }
}

void show_file_info(char *filename, struct stat *info_p) { // a function to output information on a file
    char *uid_to_name(), *ctime(), *gid_to_name(), *filemode();
    void mode_to_letters();
    char modestr[11]; // to store mode information from binary to str 

    mode_to_letters(info_p->st_mode, modestr);

    printf("%s ", modestr);
    printf("%4d ", (int)info_p->st_nlink);
    printf("%-8s ", uid_to_name(info_p->st_uid));
    printf("%-8s ", gid_to_name(info_p->st_gid));
    printf("%8ld ", (long)info_p->st_size);
    printf("%.12s ", 4+ctime(&info_p->st_mtime)); // 4+ mean to execlude days of the week
    printf("%s\n", filename); // there is no filename in stat type 
}

void mode_to_letters(int mode, char str[]) {
    strcpy(str, "----------");
	// change binary information to char
    if (S_ISDIR(mode)) str[0] = 'd';
    if (S_ISCHR(mode)) str[0] = 'c';
    if (S_ISBLK(mode)) str[0] = 'b';

    if (mode & S_IRUSR) str[1] = 'r';
    if (mode & S_IWUSR) str[2] = 'w';
    if (mode & S_IXUSR) str[3] = 'x';

    if (mode & S_IRGRP) str[4] = 'r';
    if (mode & S_IWGRP) str[5] = 'w';
    if (mode & S_IXGRP) str[6] = 'x';
    
    if (mode & S_IROTH) str[7] = 'r';
    if (mode & S_IWOTH) str[8] = 'w';
    if (mode & S_IXOTH) str[9] = 'x';
}

char *uid_to_name(uid_t uid) { // return pointer to username associated with uid
    struct passwd *getpwuid(), *pw_ptr;
    static char numstr[10];

    if((pw_ptr = getpwuid(uid)) == NULL) {
	sprintf(numstr, "%d", uid); // if error then output numstr
	return numstr;
    }
    else {
	return pw_ptr->pw_name;
    }
}

char *gid_to_name(gid_t gid) { // return pointer to group number gid
    struct group *getgrgid(), *grp_ptr;
    static char numstr[10];

    if ((grp_ptr = getgrgid(gid)) == NULL){
	sprintf(numstr,"%d", gid); // if error then output numstr
	return numstr;
    }
    else {
	return grp_ptr->gr_name;
    }
}
