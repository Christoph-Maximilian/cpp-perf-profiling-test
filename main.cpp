#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "profiling.h"

//#define INTERN_PROFILING

int fib(int x) {
    if (x == 0) return 0;
    else if (x == 1) return 1;
    return fib(x - 1) + fib(x - 2);
}

// goal is to find out why the use of perf within c++ results in completely different numbers in cache misses
// and instructions compared to use perf via the command line


// 1st scenario
// uncomment INTERN_PROFILING: `sudo ./perfstuff`,
// start perf in other terminal "perf stat -e cache-misses,instructions - `pidof perfstuff`"
// back in main terminal: press enter and wait until finished, then back to perf terminal: STRG+C

// 2nd scenario
// define INTERN_PROFILING,
// just run it, and compare the results with the previous scenario
int main() {
    std::cout << "press enter";

    std::string test;
    std::getline(std::cin, test);
    std::cout << "start";
#ifdef INTERN_PROFILING
    for (auto i = 0; i < 20; i++) {
        timeAndProfileFibonacci("", 20, fib, 40);
    }
#else
    fib(40);
#endif
    return 0;
}
