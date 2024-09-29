#include "wvector.hpp"
#include "utils.hpp"

#define LOG_SEPARATOR(msg) std::cout << "----------------------------" << std::endl;

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

void printVec(wstl::vector<int>& print_vec)
{
    for(int i = 0; i < (int)print_vec.size(); ++i) {
        LOG("vec[i] : ", print_vec[i]);
    }
}

int main()
{
    // wstl::vector<TestClass> test_class(3);

    wstl::vector<int> test_int(3);

    int arr[] = {1,2,3,4,5};
    wstl::vector<int> vec(arr, arr+sizeof(arr)/sizeof(int));

    LOG("test_copy_construct");

    wstl::vector<int> test_copy_construct(vec);
    printVec(test_copy_construct);

    // LOG("test_move_construct");
    LOG(vec.size());

    wstl::vector<int> test_move_construct;
    test_move_construct = wstl::move(vec);
    // LOG(test_move_construct.size());
    printVec(test_move_construct);

    return 0;
}