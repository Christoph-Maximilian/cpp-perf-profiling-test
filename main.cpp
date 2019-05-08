#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "PerfEvent.hpp"

#define INTERN_PROFILING

int fib(int x) {
    if (x == 0) return 0;
    else if (x == 1) return 1;
    return fib(x - 1) + fib(x - 2);
}

// 1st scenario
// uncomment (define) INTERN_PROFILING
// just run it

// 2nd scenario
// comment INTERN_PROFILING: `./perfstuff`,
// start perf in other terminal "perf stat -e cache-misses,instructions -p `pidof perfstuff`"
// back in main terminal: press enter and wait until finished, then back to perf terminal: STRG+C

// counters of 2nd scenario need to be divided by n

int main() {
    const int n = 1;
    const int x = 42;
    std::string header;
    std::string data;
#ifdef INTERN_PROFILING
    BenchmarkParameters params;
    params.setParam("name", "fib");
    {
        PerfEventBlock e(n, params, header, data, true);
        for (int i = 0; i < n; ++i)
            fib(42);
    }

    std::cout << header << std::endl;
    std::cout << data   << std::endl;

    // ALTERNATIVE USAGE:
    //PerfEvent e;
    //e.startCounters();
    //for (int i = 0; i < n; ++i)
    //  fib(42); 
    //e.stopCounters();
    //e.printReport(std::cout, n); // use n as scale factor
    //std::cout << std::endl;
#else
    std::cout << "press enter";
    std::string test;
    std::getline(std::cin, test);
    for (int i = 0; i < n; ++i)
      fib(42);
#endif
    return 0;
}
