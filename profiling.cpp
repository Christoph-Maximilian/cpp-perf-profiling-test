//
// Created by christoph on 15.12.18.
//

#ifndef JOIN_PROFILING_H
#define JOIN_PROFILING_H


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
#include "profiling.h"



PerfEvent::PerfEvent(uint64_t type, uint64_t event) {
    memset(&pe, 0, sizeof(struct perf_event_attr));
    pe.type = type;
    pe.size = sizeof(struct perf_event_attr);
    pe.config = event;
    pe.disabled = 1;
    pe.exclude_kernel = 0;
    pe.exclude_hv = 1;
    fd = syscall( __NR_perf_event_open, &pe, 0, -1, -1, 0);
    if (fd < 0)
        fprintf(stderr, "Error opening leader %llx\n", pe.config);
    ioctl(fd, PERF_EVENT_IOC_RESET, 0);
    ioctl(fd, PERF_EVENT_IOC_ENABLE, 0);
}
PerfEvent::~PerfEvent() {
    close(fd);
}
uint64_t  PerfEvent::readCounter() {
    uint64_t count;
    if (read(fd, &count, sizeof(uint64_t))!=sizeof(uint64_t))
        fprintf(stderr, "Error reading counter\n");
    return count;
}

PerfEventMT::PerfEventMT(uint64_t type, uint64_t event) {
    cpucount=sysconf(_SC_NPROCESSORS_ONLN);
    pes.reserve(cpucount);
    fd.reserve(cpucount);

    for (unsigned i=0; i<cpucount; i++) {
        struct perf_event_attr& pe=pes[i];
        memset(&pe, 0, sizeof(struct perf_event_attr));
        pe.type = type;
        pe.size = sizeof(struct perf_event_attr);
        pe.config = event;
        pe.disabled = true;
        pe.exclude_kernel = true;
        pe.exclude_hv = true;

        fd[i] = syscall( __NR_perf_event_open, &pe, -1, i, -1, 0);

        if (fd[i] < 0) {
            fprintf(stderr, "Error opening leader %llx\n", pe.config);
        }
    }

    for (unsigned i=0; i<cpucount; i++) {
        ioctl(fd[i], PERF_EVENT_IOC_RESET, 0);
        ioctl(fd[i], PERF_EVENT_IOC_ENABLE, 0);
    }
}

PerfEventMT::~PerfEventMT() {
    for (unsigned i=0; i<cpucount; i++)
        close(fd[i]);
}

uint64_t PerfEventMT::readCounter() {
    uint64_t sum=0;
    for (unsigned i=0; i<cpucount; i++) {
        uint64_t count;
        if (read(fd[i], &count, sizeof(uint64_t))!=sizeof(uint64_t))
            fprintf(stderr, "Error reading counter\n");
        sum+=count;
    }
    return sum;
}



#endif //JOIN_PROFILING_H
