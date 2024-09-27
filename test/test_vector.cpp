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

int main()
{
    wstl::vector<TestClass> test_class(3);

    wstl::vector<int> test_int(3);

    int arr[] = {1,2,3,4,5};
    wstl::vector<int> vec(arr, arr+sizeof(arr)/sizeof(int));

    for(int i = 0; i < (int)vec.size(); ++i) {
        LOG("vec[i] : ", vec[i]);
    }

    return 0;
}