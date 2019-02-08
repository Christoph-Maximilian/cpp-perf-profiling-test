//
// Created by christoph on 08.02.19.
//

//
// Created by christoph on 15.12.18.
//

#include <iostream>
#include <cstring>
#include <string>
#include <sys/time.h>
#include <functional>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/perf_event.h>
#include <asm/unistd.h>
#include <vector>

class PerfEvent {
    struct perf_event_attr pe;
    int fd;
public:
    PerfEvent(uint64_t type, uint64_t event);
    ~PerfEvent();
    uint64_t readCounter();
};

class PerfEventMT {
    std::vector<struct perf_event_attr> pes;
    std::vector<int> fd;
    unsigned cpucount;

public:
    PerfEventMT(uint64_t type, uint64_t event);

    ~PerfEventMT();

    uint64_t readCounter();
};


static inline double gettime(void) {
    struct timeval now_tv;
    gettimeofday (&now_tv, NULL);
    return ((double)now_tv.tv_sec) + ((double)now_tv.tv_usec)/1000000.0;
}

/*
 * PERF_COUNT_HW_CACHE_L1D			= 0,
	PERF_COUNT_HW_CACHE_L1I			= 1,
	PERF_COUNT_HW_CACHE_LL			= 2,
	PERF_COUNT_HW_CACHE_DTLB		= 3,
	PERF_COUNT_HW_CACHE_ITLB		= 4,
	PERF_COUNT_HW_CACHE_BPU			= 5,
 */

static void timeAndProfileFibonacci(std::string s, uint64_t n, std::function<void (uint32_t)> fn, uint32_t until) {
    PerfEvent misses(PERF_TYPE_HARDWARE, PERF_COUNT_HW_CACHE_MISSES);
    PerfEvent instructions(PERF_TYPE_HARDWARE, PERF_COUNT_HW_INSTRUCTIONS);
    double start = gettime();
    fn(until);
    double end = gettime();

    std::string m =  std::to_string(misses.readCounter()/1000000) + " M misses\n";
    //<< ((n/1e6)/(end-start))
    std::cout << s << std::to_string(misses.readCounter()) << "cache misses - "  << std::to_string(instructions.readCounter()/(float)n) << " instructions " << std::endl;
    //std::cout << m << m2 << m3 << m4 <<m5 << m6;
}
