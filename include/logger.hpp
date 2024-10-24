#ifndef LOGGER_HPP__
#define LOGGER_HPP__

/**
 * @file logger.hpp
 * @brief Responsible for logging
 * @author wangqinghe
 * @date 9/27/2021
 * @version 1.0
 * 
 * Copyright © Luis. All rights reserved.
 */

/************ Logger ******************/

#include <ctime>
#include <chrono>
#include <iomanip>
#include <string>
#include <iostream>

enum class LogLevel
{
    DEBUG = 0,
    INFO,
    WARNING,
    ERROR
};

#define SHOW_LEVEL LogLevel::INFO

#define LOG_SEPARATOR_START() std::cout << "-----------" << __func__ <<  " start -----------------" << std::endl;
#define LOG_SEPARATOR_END() std::cout << "-----------" << __func__ <<  " end -----------------" << std::endl;
#define LOG(LEVEL, ...) Logger::getInstance().printLog(__FILE__, LEVEL, __LINE__, __func__, __VA_ARGS__ )
#define LOGD(...) LOG(LogLevel::DEBUG, __VA_ARGS__)
#define LOGI(...) LOG(LogLevel::INFO, __VA_ARGS__)
#define LOGW(...) LOG(LogLevel::WARNING, __VA_ARGS__)
#define LOGE(...) LOG(LogLevel::ERROR, __VA_ARGS__)

class Logger {
public:
public:
    static Logger& getInstance() {
        static Logger logger;
        return logger;
    }

    template<typename... Args>
    void printLog(const std::string& log_file, LogLevel log_level, const long log_line, const std::string& log_func, Args... args) {
        if(log_level < SHOW_LEVEL) return;
        std::ostringstream oss;
        oss << getCurrentTime() << getLogLevel(log_level);
        oss << "[" << log_file << ":" << log_line << " " << log_func << "] ";
        print_helper_C11(oss, args...);
        std::cout << oss.str();
    }

    std::string getLogLevel(LogLevel level) {
        switch (level)
        {
        case LogLevel::DEBUG: return "[DEBUG] ";
        case LogLevel::INFO: return "[INFO] ";
        case LogLevel::WARNING: return "[WARNING] ";
        case LogLevel::ERROR: return "[ERROR] ";
        default: return "[UNKNOWN] ";
        }
    }

    std::string getCurrentTime() {
        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);

        std::tm local_tm;
        localtime_r(&now_time, &local_tm);

        std::ostringstream oss;
        oss << std::put_time(&local_tm, "[%Y-%m-%d %H:%M:%S]");
        return oss.str();
    }

private:
    /*** C++17 ******************
    template<typename T, typename... Args>
    static void print_helper(std::ostringstream& oss, T firstArg, Args... args) {
        oss << firstArg;
        if constexpr (sizeof...(args) > 0) {
            oss << " ";
            print_helper(oss, args...);
        }
    }
    ********** C++17 ***********/

    /** C++11 ****/
    template<typename T, typename... Args>
    void print_helper_C11(std::ostringstream& oss, const T& t, const Args&... args) {
        oss << t << " ";
        print_helper_C11(oss, args...);
    }

    template<typename T>
    void print_helper_C11(std::ostringstream& oss, const T& t) {
        oss << t << std::endl;
    }
};

/************ Logger ******************/

#endif

/**
 * [day03] split logger.hpp from utils.hpp
 */