#ifndef OSS_UTIL_HPP__
#define OSS_UTIL_HPP__

#include "core.hpp"

inline void ossSleepMicros(unsigned int s) {
    struct timespec t;
    t.tv_sec = (time_t) (s / 1000000);
    t.tv_nsec = 1000 * (s % 1000000);
    while (nanosleep(&t,&t) == -1 && errno == EINTR);
}

inline void ossSleepMillis(unsigned int s) {
    ossSleepMicros(s * 1000);
}

typedef pid_t OSS_PID;
typedef pthread_t OSS_TID;

inline OSS_PID ossGetParentProcessID() {
    return getppid();
}

inline OSS_PID ossGetProcessID() {
    return getpid();
}

inline OSS_TID ossGetCurrentThreadID() {
    return syscall(SYS_gettid);
}

#endif