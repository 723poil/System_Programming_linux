/*
file       : who3.c
author     : 2017110051_이상협
datetime   : 2021-09-11 15:59
decription : utmp file을 opend하여 user접속기록을 확인하기 위한 코드로,
            utmplib파일을 통해 utmp파일을 열고, 읽고, 닫는 함수를 이용하여 데이터를 불러옴
	    utmplib의 함수는 헤더파일을 통해 가져왔으며, 날짜를 0000-00-00 00:00 형식으로 출력하기 위해
	    localtime 함수와 tm 구조체를 사용하였음.
	    showtime 함수에 들어갈때 time_t형식을 사용해서 코드를 작성해보려 했으나, 아직 미숙하여
	    long 형식으로 넣고 안에서 time_t형식으로 변환을 함
*/

#include <stdio.h>
#include <sys/types.h>
#include <utmp.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>
#include "utmplib.h"       // utmplib 함수를 사용하기위해 헤더파일 선언

#define SHOWHOST

#define EMPTY 0            // 필요할 것 같아서 삽입
#define RUN_LVL 1
#define BOOT_TIME 2
#define OLE_TIME 3
#define NEW_TIME 4
#define INIT_PROCESS 5
#define LOGIN_PROcESS 6
#define USER_PROCESS 7
#define DEAD_PROCESS 8

void showtime(long);           // 함수의 좀더 수월한 호출을 위해 위치 바꿈
void show_info( struct utmp*);

int main() {
    struct utmp *utbufp, *utmp_next(); // 현재 버퍼 주소, 다음 버퍼 주소

    if ( utmp_open( UTMP_FILE ) == -1 ) { // 파일이 열리지 않으면 error 출력
	perror(UTMP_FILE);
	exit(1);
    }
    
    while ( ( utbufp = utmp_next() ) != ( ( struct utmp * ) NULL ) ) { // 사용자 접속상태 출력
	show_info( utbufp );
    }

    utmp_close(); // 출력 후 파일 닫기
    return 0;
}

void showtime(long timeval) {
    struct tm *t;          // tm 구조체 선언
    t = localtime(&timeval); // tm 구조체에 접속시간 삽입
    printf("%d-%02d-%02d %02d:%02d", 1900+t->tm_year, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min);
}                         // 0000-00-00 00:00 형식으로 출력

void show_info(struct utmp * utbufp) {
    if ( utbufp->ut_type != USER_PROCESS ) { 
	return;
    }

    printf("%-11.11s", utbufp->ut_name); // 이름이 길어서 조금 더 길게 출력
    printf(" ");
    printf("%-8.8s", utbufp->ut_line);
    printf(" ");
    showtime( utbufp->ut_time ); // 접속 시간 출력 함수

#ifdef SHOWHOST
    if (utbufp->ut_host[0] != '\0' ) {
	printf("(%s)", utbufp->ut_host);
    }

#endif
    printf("\n");
}
