#ifndef TIMER_H
#define TIMER_H

#ifdef __linux__
#include <sys/time.h>
#else
#include <time.h>
#endif
#include <stdio.h>

double getTime(){
#ifdef __linux__
    timeval tv;
    gettimeofday(&tv, 0);
    return tv.tv_sec + tv.tv_usec * 1e-6;
#else
    return 1.0 * clock() / CLOCKS_PER_SEC;
#endif
}

#define tic tic_f(__LINE__);
#define toc toc_f(__LINE__);

#define MAX_LINE 10000

double _timer[MAX_LINE] = {0};
int _fromLine[MAX_LINE] = {0};
int _lastLineNum = -1;
double _lastRdtsc = 0;

inline void tic_f(int line){
    _lastLineNum = line;
    _lastRdtsc = getTime();
}

inline void toc_f(int line){
    double t = getTime();
    _fromLine[line] = _lastLineNum;
    _timer[line] += t - _lastRdtsc;
    _lastLineNum = line;
    _lastRdtsc = t;
}

inline void tictoc(FILE *out){
    int i;
    for(i = 0; i < MAX_LINE; i++){
        if(_timer[i] != 0)
            fprintf(out, "line %d - %d: %.3f\n", _fromLine[i], i, _timer[i]);
    }
    fflush(out);
}

#endif
