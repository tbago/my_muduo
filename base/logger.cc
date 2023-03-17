#include "logger.h"
#include "time_stamp.h"

namespace base {

Logger &Logger::Instance() {
    static Logger logger;
    return logger;
}

void Logger::SetLogLevel(LogLevel log_level) {
    log_level_ = log_level;
}

void Logger::Log(LogLevel level, std::string message) {
    if (level < log_level_) {
        return;
    }

    std::string type = "";
    switch (level) {
        case LogLevel::DEBUG:
            type = "[DEBUG]";
            break;
        case LogLevel::INFO:
            type = "[INFO]";
            break;
        case LogLevel::ERROR:
            type = "[ERROR]";
            break;
        case LogLevel::FATAL:
            type = "[FATAL]";
            break;
    }
    fprintf(stderr, "%s %s %s\n", type.c_str(), TimeStamp::Now().ToString().c_str(), message.c_str());
}

}  // namespace base
