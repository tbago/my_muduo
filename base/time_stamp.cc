#include "time_stamp.h"

#include <chrono>

namespace base {

TimeStamp TimeStamp::Now() {
    return TimeStamp(time(NULL));
}

TimeStamp::TimeStamp() : micro_seconds_since_epoch_(0) {
}

TimeStamp::TimeStamp(int64_t micro_seconds_since_epoch) : micro_seconds_since_epoch_(micro_seconds_since_epoch) {
}

std::string TimeStamp::ToString() const {
    char buf[128] = {0};
    time_t *time = (time_t *)&micro_seconds_since_epoch_;
    tm *tm_time = localtime(time);
    snprintf(buf, 128, "%4d/%02d/%02d %02d:%02d:%02d",
             tm_time->tm_year + 1900,
             tm_time->tm_mon + 1,
             tm_time->tm_mday,
             tm_time->tm_hour,
             tm_time->tm_min,
             tm_time->tm_sec);
    return buf;
}

}  // namespace base
