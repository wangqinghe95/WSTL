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

#define DEBUG_VERSION "20240926_01"
#define LOG(...) Log::getInstance().printLog(__FILE__, __LINE__, __func__, __VA_ARGS__)

class Log {
public:
public:
    static Log& getInstance() {
        static Log logger;
        return logger;
    }

    template<typename... Args>
    void printLog(const std::string& log_file, const long log_line, const std::string& log_func, Args... args) {
        std::ostringstream oss;
        oss << "[" << log_file << ":" << log_line << " " << log_func << "] ";
        print_helper_C11(oss, args...);
        std::cout << oss.str();       
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