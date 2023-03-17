#pragma once

#include <sstream>
#include <string>

#include "none_copyable.h"

namespace base {

#define LOG_INFO(format, ...)                                                   \
    do {                                                                        \
        std::stringstream stream;                                               \
        stream << "[" << __FILE__ << ":" << __LINE__ << " " << __func__ << "()" \
               << "] ";                                                         \
        char buf[1024] = {0};                                                   \
        snprintf(buf, 1024, format, ##__VA_ARGS__);                             \
        Logger::Instance().Log(INFO, stream.str() + buf);                       \
    } while (0)

#define LOG_ERROR(format, ...)                                                  \
    do {                                                                        \
        std::stringstream stream;                                               \
        stream << "[" << __FILE__ << ":" << __LINE__ << " " << __func__ << "()" \
               << "] ";                                                         \
        char buf[1024] = {0};                                                   \
        snprintf(buf, 1024, format, ##__VA_ARGS__);                             \
        Logger::Instance().Log(ERROR, stream.str() + buf);                      \
    } while (0)

#define LOG_FATAL(format, ...)                                                  \
    do {                                                                        \
        std::stringstream stream;                                               \
        stream << "[" << __FILE__ << ":" << __LINE__ << " " << __func__ << "()" \
               << "] ";                                                         \
        char buf[1024] = {0};                                                   \
        snprintf(buf, 1024, format, ##__VA_ARGS__);                             \
        Logger::Instance().Log(FATAL, stream.str() + buf);                      \
        exit(-1);                                                               \
    } while (0)

#define LOG_DEBUG(format, ...)                                                  \
    do {                                                                        \
        std::stringstream stream;                                               \
        stream << "[" << __FILE__ << ":" << __LINE__ << " " << __func__ << "()" \
               << "] ";                                                         \
        char buf[1024] = {0};                                                   \
        snprintf(buf, 1024, format, ##__VA_ARGS__);                             \
        Logger::Instance().Log(FATAL, stream.str() + buf);                      \
    } while (0)

// 定义日志的级别 DEBUG INFO ERROR FATAL
enum LogLevel {
    DEBUG,  // 调试信息
    INFO,   // 普通信息
    ERROR,  // 错误信息
    FATAL,  // 严重错误
};

class Logger : private NoneCopyable {
public:
    // 获取日志唯一的实例对象 单例
    static Logger &Instance();
    // 设置日志级别，只有大于等于日志级别的日志才会被记录
    void SetLogLevel(LogLevel log_level);
    // 写日志
    void Log(LogLevel level, std::string message);

private:
    Logger(){};
    ~Logger(){};

private:
    LogLevel log_level_;
};

}  // namespace base
