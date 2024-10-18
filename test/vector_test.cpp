#include "wvector.hpp"
#include "utils.hpp"

class TestClass
{
public:
    TestClass() : num(8){
        LOG(DEBUG_VERSION);
    }

    friend std::ostream& operator<<(std::ostream& os, const TestClass& _class);
private:
    int num;
};

std::ostream& operator<<(std::ostream& os, const TestClass& _class) {
    os << "Num: " << _class.num << std::endl;
    return os;
}

void printVec(const std::string& log, wstl::vector<int>& print_vec)
{
    std::cout << log << " start" << std::endl;
    for(int i = 0; i < (int)print_vec.size(); ++i) {
        std::cout << print_vec[i] << "\t";
    }
    std::cout << "\n";
    std::cout << log << " end" << std::endl;
}

void testEmplace()
{
    LOG_SEPARATOR_START();
    wstl::vector<int> vec;
    vec.emplace(vec.begin(), 2);
    printVec("test emplace", vec);
    LOG_SEPARATOR_END();
}

void testResize()
{
    LOG_SEPARATOR_START();
    wstl::vector<int> vec{1,2,4,5};

    vec.resize(10,5);
    printVec("Expand vec size", vec);

    vec.resize(7);
    printVec("Expand Vec size with default value", vec);

    vec.resize(3);
    printVec("Reduce Vec size", vec);
    LOG_SEPARATOR_END();
}

void testInsert()
{
    LOG_SEPARATOR_START();
    wstl::vector<int> vec{1,2,3};
    vec.insert(vec.begin() + 1, 42);
    printVec("insert single element(left value)", vec);

    vec.insert(vec.begin()+2, 2, 77);
    printVec("insert multiple same element", vec);

    wstl::vector<int> vec2{100,200};
    vec.insert(vec.begin()+5, vec2.begin(), vec2.end());
    printVec("insert iterator range element", vec);

    LOG_SEPARATOR_END();
}

int main()
{
    testInsert();
    // testEmplace();
    // testResize();
    return 0;
}