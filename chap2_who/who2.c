#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#define SHOWHOST

#define EMPTY 0
#define RUN_LVL 1
#define BOOT_TIME 2
#define OLE_TIME 3
#define NEW_TIME 4
#define INIT_PROCESS 5
#define LOGIN_PROCESS 6
#define USER_PROCESS 7
#define DEAD_PROCESS 8

void showtime(long);
void show_info(struct utmp*);

int main() {
    struct utmp current_record;
    int utmpfd;
    int reclen = sizeof(current_record);

    if ( (utmpfd = open(UTMP_FILE, O_RDONLY) ) == -1) {
	perror( UTMP_FILE );
	exit(1);
    }

    while ( read(utmpfd, &current_record, reclen) == reclen ) {
	show_info(&current_record);
    }

    close(utmpfd);

    return 0;
}

void showtime( long timeval ) {
    char *cp;
    cp = ctime(&timeval);

    printf("%12.12s", cp+4 );
}

void show_info( struct utmp* utbufp) {

    if ( utbufp->ut_type != USER_PROCESS ) {
	return;
    }

    printf("%-8.8s", utbufp->ut_name);
    printf(" ");
    printf("%-8.8s", utbufp->ut_line);
    printf(" ");
    showtime( utbufp->ut_time );

#ifdef SHOWHOST
    if ( utbufp->ut_host[0] != '\0' )
        printf("(%s)", utbufp->ut_host);
#endif
    printf("\n");
}
