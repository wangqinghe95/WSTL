#include "logger.hpp"
#include <iostream>

int main()
{
    // std::cout << Log::getInstance().getCurrentTime() << std::endl;
    LOGD("test LOGD");
    LOGD("test LOGD2");
    return 0;
}
