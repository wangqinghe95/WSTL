#include "wconstruct.hpp"
#include "wvector.hpp"
#include "utils.hpp"
#include <iostream>
using namespace std;
using namespace wstl;

#define VOID ""
#define TESTDATE "20240924_01"

class testClass
{
public:
    testClass() {
        // LOGI(TESTDATE);
    }
};

void testConstruct()
{
    char buffer[sizeof(testClass)];
    testClass *p = reinterpret_cast<testClass*>(buffer);
    construct(p);
}

void testVector()
{
    vector<int> v_int;
}

void testLog()
{
    const char* p = "test";
    LOG(p);
    // LOGNON();
}

int main()
{
    testConstruct();
    testVector();
    testLog();
    return 0;
}