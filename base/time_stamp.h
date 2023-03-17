#pragma once
#include <string>

namespace base {

class TimeStamp {
public:
    static TimeStamp Now();

public:
    TimeStamp();
    explicit TimeStamp(int64_t micro_seconds_since_epoch_);
    std::string ToString() const;

private:
    int64_t micro_seconds_since_epoch_;
};

}  // namespace base
