#include <iostream>
#include "base/time_stamp.h"
#include "base/logger.h"

using namespace base;

int main(int argc, const char *argv[]) {
    // std::cout << TimeStamp::Now().ToString() << std::endl;
    LOGI("message from main");
    return 0;
}
