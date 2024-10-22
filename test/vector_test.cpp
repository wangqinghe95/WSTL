#include "wvector.hpp"
#include "utils.hpp"

#define DEBUG_DATE "20241021_01"


class TestClass
{
public:
    TestClass() : num(8){
        // LOG(DEBUG_VERSION);
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

void testConstructor()
{
    // default construct
    // vector()
    wstl::vector<int> default_vec;
    // LOG("default_vec.size():", default_vec.size());
    // LOG("default_vec.capacity():", default_vec.capacity());

    // vector(n);
    wstl::vector<int> vec_n(8);
    printVec("vec construct with n:", vec_n);

    // vector(n, value);
    wstl::vector<int> vec_n_value(8,2);
    printVec("vec constrcut with n tiems value:", vec_n_value);

    // vector(vector.begin(), vector.end())
    wstl::vector<int> vec_iterator_copy(vec_n_value.begin(), vec_n_value.end());
    printVec("vec_iterator_copy copy from vec_n_value", vec_iterator_copy);

    // vector(initializer_list)
    wstl::vector<int> original_vec{1,2,3};
    printVec("initializer_vector,", original_vec);

    // vector(vector&)
    wstl::vector<int> copy_vector(original_vec);
    printVec("copy constructor", copy_vector);

    // vector(vector&&)
    wstl::vector<int> move_vector(wstl::move(original_vec));
    printVec("move constructor", move_vector);
    LOGI("original_vec capacity : ", original_vec.capacity());
    LOGI("original_vec size : ", original_vec.size());

    // operator=
    wstl::vector<int> vector_assign_origin{1,2,3,5,7};
    wstl::vector<int> vector_assign;
    vector_assign = vector_assign_origin;
    printVec("operator= ", vector_assign);

    // operator= move
    wstl::vector<int> vector_assign_move = wstl::move(vector_assign_origin);
    printVec("vector_assign_origin:", vector_assign_origin);
    printVec("vector_assign:", vector_assign);
    printVec("vector_assign_move:", vector_assign_move);
    LOGI("after operator= move: ", vector_assign_origin.size());
}

void printVecWithIterator(const wstl::vector<int>& vec)
{
    for(auto it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << '\t';
    }
    std::cout << std::endl;

    for(auto it = vec.rbegin(); it != vec.rend(); ++it) {
        std::cout << *it << '\t';
    }
    std::cout << std::endl;

    for(auto it = vec.cbegin(); it != vec.cend(); ++it) {
        std::cout << *it << '\t';
    }
    std::cout << std::endl;

    for(auto it = vec.crbegin(); it != vec.crend(); ++it) {
        std::cout << *it << '\t';
    }
    std::cout << std::endl;

}

void testIterator()
{
    wstl::vector<int> vec{1,2,3,4,5,6};
    printVecWithIterator(vec);

    const wstl::vector<int> vec_const{7,8,9,10};
    printVecWithIterator(vec_const);
}

void testCapacity()
{
    wstl::vector<int> vec;
    LOGI(vec.empty() == true ? "vec is empty" : "vec is not empty");

    wstl::vector<int> vec1{1,2,3,4,5};
    LOGI("vec1.size : ", vec1.size());
    LOGI("vec1.maxsize : ", vec1.max_size(), " vec1.capacity: ", vec1.capacity());
    // LOGI("vec1.capacity: ", vec1.capacity());

    vec1.reserve(20);
    LOGI("vec1.maxsize : ", vec1.max_size(), " vec1.capacity: ", vec1.capacity());

    LOGI("vec[3]:", vec1[3], "vec.at(2)", vec1.at(2));

    const wstl::vector<int> const_vec = vec1;
    LOGI("const vec[4]:", const_vec[4], "const vec.at(2)", const_vec.at(2));

    LOGI("vec.front:", vec1.front(), "cosnt vec.front:", const_vec.front());
    LOGI("vec.back:", vec1.back(), "cosnt vec.back:", const_vec.back());
    LOGI("vec.data:", *vec1.data(), "cosnt vec.data:", *const_vec.data());
}

void testAssign()
{
    wstl::vector<int> source{1,2,3,4,5};
    wstl::vector<int> vec;
    vec.assign(source.begin(), source.end());
    assert(vec.size() == source.size());
    for(size_t i = 0; i < vec.size(); ++i) {
        assert(vec[i] == source[i]);
    }

    LOGI("assign (iterator) test passed!");

    wstl::vector<int> vec1;
    vec1.assign(5,42);
    assert(vec1.size() == 5);
    for(size_t i = 0; i < vec1.size(); ++i) {
        assert(vec1[i] == 42);
    }
    LOGI("assign (size and value) test passed!");

    wstl::vector<int> vec_list({1,2,3,4,5});
    for(size_t i = 0; i < vec_list.size(); ++i) {
        assert(static_cast<size_t>(vec_list[i]) == i+1);
    }
    LOGI("assign (initializer list) test passed!");
}

void testErase()
{
    wstl::vector<int> vec = {1,2,3,4,5};
    vec.erase(vec.begin() + 1, vec.begin() + 3);
    assert(vec.size() == 3);

    wstl::vector<int> vec_erase = {1,4,5};

    for(size_t i = 0; i < vec.size(); ++i) {
        assert(vec[i] == vec_erase[i]);
    }

    vec.erase(vec.begin() + 1);
    assert(vec.size() == 2);

    vec.clear();
    assert(vec.empty());

    LOGI("erase (range) test passed!");
}

int main()
{
    LOGI(DEBUG_DATE);
    // testConstructor();
    // testIterator();
    // testCapacity();
    // testAssign();
    // testInsert();
    // testEmplace();
    // testResize();
    testErase();
    return 0;
}