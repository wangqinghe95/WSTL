#ifndef TEST_COMMON_HPP__
#define TEST_COMMON_HPP__

#include "utils.hpp"

#define DEBUG_DATE "20241031_01"

class TestClass
{
public:
    TestClass() : num(8){
        // LOGI(DEBUG_DATE);
    }

    TestClass(int v) : num(v){
        LOGI(DEBUG_DATE);
    }

    friend std::ostream& operator<<(std::ostream& os, const TestClass& _class);

    bool operator==(const TestClass& other) const {
        return num == other.num;
    }

    bool operator!=(const TestClass& other) const {
        return !(*this == other);
    }

    bool operator<(const TestClass& other) const {
        return num < other.num;
    }

    bool operator>(const TestClass& other) const {
        return other < *this;
    }

    bool operator<=(const TestClass& other) const {
        return !(other < *this);
    }

    bool operator>=(const TestClass& other) const {
        return !(*this < other);
    }

    inline int getNum() {
        return num;
    }
private:
    int num;
};

std::ostream& operator<<(std::ostream& os, const TestClass& _class) {
    os << "Num: " << _class.num << std::endl;
    return os;
}


#endif