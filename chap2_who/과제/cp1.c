/*
file        : cp1.c
author      : 2017110051_이상협
datetime    : 2021-09-11 14:55
description : 파일을 copy하기 위한 c파일로 복사할 파일과 덮어쓰기 할 파일을 열어서
             지정한 버퍼사이즈만큼 버퍼를 읽으면서 덮어쓰기 할 파일에 데이터를 복사하는 코드 
*/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define BUFFERSIZE 4096
#define COPYMODE 0644 // 8진수 110 / 100 / 100

void oops(char*, char*); // error output

int main(int ac, char *av[]) { // ac -> 인자의 개수, av -> 인자
    int in_fd, out_fd, n_chars; // 복사하려는 파일과 붙여넣을 파일의 file descriptor
    char buf[BUFFERSIZE];

    if ( ac != 3 ) { // 인자가 제대로 입력되었는지 체크
	fprintf( stderr, "usage: %s source destination\n", *av);
	exit(1);
    }

    if ( ( in_fd = open(av[1], O_RDONLY) ) == -1 ) { // open file
	oops( "Cannot open ", av[1]);
    }

    if ( ( out_fd = creat(av[2], COPYMODE) ) == -1 ) { // create or open file
	oops( "Cannot creat", av[2]);
    }

    while ( (n_chars = read(in_fd, buf, BUFFERSIZE)) > 0 ) { // in_fd 파일에서 BUFFERSIZE만큼 데이터를 read해서
	if ( write( out_fd, buf, n_chars ) != n_chars ) {    // out_fd 파일에 write
	    oops( "Read error from ", av[2]);
	}
    }

    if ( n_chars == -1 ) { // 잘못 읽었을 경우 error 출력
	oops("Read error from ", av[1]);
    }

    if (close(in_fd) == -1 || close(out_fd)== -1) { // 완료 후 정상적으로 파일이 닫혔는지 확인
	oops("Error closing files", "");
    }

    return 0;
}

void oops(char *s1, char *s2) { // error가 발생했을 경우 추가 문구와 함께 error문구 출력
    fprintf(stderr, "Error: %s ", s1);
    perror(s2);
    exit(1);
}
