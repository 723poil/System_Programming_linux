/*
file        : set_ticker.c
author      : 2017110051_이상협
datetime    : 2021-10-15 16:30
description : the code that sets up if you put in the time to repeat it every certain time.
*/
#include <sys/time.h>
#include <stdio.h>

int set_ticker(int n_msecs) {
    
    struct itimerval new_timeset;
    long n_sec, n_usecs;  // second, microseconds

    n_sec = n_msecs / 1000;                  // second int part
    n_usecs = (n_msecs % 1000) * 1000L;      // remainder -> microseconds

    new_timeset.it_interval.tv_sec = n_sec;    // set record
    new_timeset.it_interval.tv_usec = n_usecs; // new ticker interval  value 

    new_timeset.it_value.tv_sec = n_sec;       // set record
    new_timeset.it_value.tv_usec = n_usecs;    // new ticker first value

    return setitimer(ITIMER_REAL, &new_timeset, NULL); // return setting value
}
