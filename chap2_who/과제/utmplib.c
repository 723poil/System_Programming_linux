/*
file        : utmplib.c
author      : 2017110051_이상협
datetime    : 2021-09-11 15:32
description : utmp file을 열어 buffer를 읽는 기능을 함
              utmp_open 함수로 파일을 열고
	      utmp_next 함수로 다음 rec의 접속 정보를 불러오고
	      utmp_close 함수를 통해 열었던 utmp file을 닫는다.
*/

#include <stdio.h>
#include <fcntl.h>      // open함수 사용
#include <unistd.h>     // read, close함수 사용
#include <sys/types.h>
#include <utmp.h>

#define NRECS 16 // user_range
#define NULLUT ((struct utmp *)NULL)
#define UTSIZE (sizeof(struct utmp))

// 전역 변수
static char utmpbuf[NRECS * UTSIZE]; // 버퍼저장
static int num_recs;                 // 몇명의 사용자가 사용하고있는지
static int cur_rec;                  // 현재 사용자 ( number )
static int fd_utmp = -1;             // file descriptor 정보 저장할 변수

int utmp_open( char *filename ) {
    fd_utmp = open( filename, O_RDONLY ); // open
    cur_rec = num_recs = 0;               // default
    return fd_utmp;                       // return
}

int utmp_reload() { // 중요!!
    int amt_read;

    amt_read = read( fd_utmp, utmpbuf, NRECS * UTSIZE ); // 남은 데이터들 read

    num_recs = amt_read/UTSIZE; // 읽은 buf 만큼에서 utmp struct size 만큼 나누기

    cur_rec = 0; // cur_rec 초기화

    return num_recs;
}

struct utmp *utmp_next() {
    struct utmp *recp;
    if ( fd_utmp == -1 ) {                // file을 제대로 열었는지
	return NULLUT;
    }
    
    if ( cur_rec == num_recs && utmp_reload()==0 ) { // 전부 검색하고, 더이상 가져올 데이터가 없을 경우
	return NULLUT;
    }

    recp = ( struct utmp * )&utmpbuf[cur_rec * UTSIZE]; // get adr of nxt rec
    cur_rec++;
    return recp;
}

void utmp_close() {
    if ( fd_utmp != -1 ) { // 제대로 열었을 경우 파일 닫기
	close( fd_utmp );
    }
}

