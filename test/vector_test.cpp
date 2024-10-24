#include "wvector.hpp"
#include "utils.hpp"

#define DEBUG_DATE "20241023_01"


class TestClass
{
public:
    TestClass() : num(8){
        // LOG(DEBUG_VERSION);
    }

    TestClass(int v) : num(v){}

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
    wstl::vector<TestClass> vec;

    // test emplace_back
    vec.emplace_back(1);
    assert(vec.size() == 1 && "emplace_back failed");
    assert(vec[0].getNum() == 1 && "emplace_back insert wrong value");

    // test emplace any position
    vec.emplace(vec.begin()+1, 2);
    assert(vec.size() == 2 && "emplace at any position failed");
    assert(vec[1].getNum() == 2 && "emplace at any position value wrong value");

    // test emplace end
    vec.emplace(vec.end(), 3);
    assert(vec.size() == 3 && "emplace at any position failed");
    assert(vec[2].getNum() == 3 && "emplace at any position value wrong value");

    LOGI("test emplace passed!");

}

void testResize()
{
    wstl::vector<int> vec{1,2,4,5};

    vec.resize(7);
    // printVec("Expand Vec size with default value", vec);
    assert(vec.size() == 7 && vec.at(6) == 0 && "vector resize(size) failed");

    vec.resize(10,5);
    assert(vec.size() == 10 && vec.at(9) == 5 && "vector resize(size, value) failed");

    vec.resize(3);
    assert(vec.size() == 3 && "vector resize(reduce) failed");

    LOGI("vector resize passed!");
}

void testInsert()
{
    wstl::vector<int> vec{1,2,3};
    vec.insert(vec.begin() + 1, 42);
    // printVec("insert single element(left value)", vec);
    assert(vec.size() == 4 && vec.at(1) == 42 && "vector insert value failed");

    vec.insert(vec.begin()+2, 2, 77);
    // printVec("insert multiple same element", vec);

    assert(vec.size() == 6 && vec.at(2) == 77 && vec.at(3) == 77 && "vector insert range failed");

    wstl::vector<int> vec2{100,200};
    vec.insert(vec.begin()+5, vec2.begin(), vec2.end());
    // printVec("insert iterator range element", vec);
    assert(vec.size() == 8 && vec.at(5) == 100 && vec.at(6) == 200 && "vector insert vector failed");

    LOGI("vector insert passed!");
}

void testConstructor()
{
    // default construct
    // vector()
    wstl::vector<int> default_vec;
    assert(default_vec.size() == 0 && default_vec.capacity() != 0 && "default construct failed");

    // vector(n);
    wstl::vector<int> vec_n(8);
    assert(vec_n.size() == 8 && "vector(n) failed");

    // vector(n, value);
    wstl::vector<int> vec_n_value(8,2);
    assert(vec_n_value.size() == 8 && "vector(n,value) size failed");
    for(size_t i = 0; i < vec_n.size(); ++i) {
        assert(vec_n_value[i] == 2 && "vector(n,value) value failed");
    }

    // vector(vector.begin(), vector.end())
    wstl::vector<int> vec_iterator_copy(vec_n_value.begin(), vec_n_value.end());
    assert(vec_n_value.size() == vec_iterator_copy.size() && "vector(vector.begin(), vector.end()) size failed");
    for(size_t i = 0; i < vec_iterator_copy.size(); ++i) {
        assert(vec_iterator_copy[i] == 2 && "vector(vector.begin(), vector.end()) value failed");
    }

    // vector(initializer_list)
    wstl::vector<int> original_vec{1,2,3};
    assert(original_vec.size() == 3 && "vector(initializer_list) size failed");
    for(size_t i = 0; i < original_vec.size(); ++i) {
        assert(original_vec[i] == static_cast<int>(i+1) && "vector(initializer_list) value failed");
    }

    // vector(vector&)
    wstl::vector<int> copy_vector(original_vec);
    assert(original_vec.size() == copy_vector.size() && "vector(vector&) size failed");
    for(size_t i = 0; i < copy_vector.size(); ++i) {
        assert(copy_vector[i] == static_cast<int>(i+1) && "vector(vector&) value failed");
    }

    // vector(vector&&)
    wstl::vector<int> move_vector(wstl::move(original_vec));
    assert(move_vector.size() == 3 && "vector(vector&&) size failed");
    assert(original_vec.size() == 0 && "move failed");
    for(size_t i = 0; i < copy_vector.size(); ++i) {
        assert(move_vector[i] == static_cast<int>(i+1) && "vector(vector&&) value failed");
    }

    // operator=
    wstl::vector<int> vector_assign_origin{1,2,3,4,5};
    wstl::vector<int> vector_assign;
    vector_assign = vector_assign_origin;
    assert(vector_assign.size() == vector_assign_origin.size() && "vector_assign size failed");
    for(size_t i = 0; i < vector_assign.size(); ++i) {
        assert(vector_assign_origin[i] == vector_assign[i] && "vector(vector&&) value failed");
    }

    // operator= move
    wstl::vector<int> vector_assign_move = wstl::move(vector_assign_origin);
    assert(vector_assign_move.size() == 5 && "operator= move size failed");
    assert(vector_assign_origin.size() == 0 && "operator= move failed");
    for(size_t i = 0; i < copy_vector.size(); ++i) {
        assert(move_vector[i] == static_cast<int>(i+1) && "operator= move value failed");
    }
    LOGI("vector constructor passed!");
}

void testIteratorDetail()
{
    wstl::vector<int> vec{1,2,3,4,5};

    // test iterator invalid
    assert(vec.begin() != vec.end() && "vec.begin() or vec.end() error");
    assert(*vec.begin() == 1 && "*begin() failed");
    assert(*(vec.end()-1) == 5 && "*end() failed");

    auto it = vec.begin();
    ++it;
    assert(*it == 2 && "it++ failed");

    it = vec.end();
    --it;
    assert(*it == 5 && "it--");

    assert(vec.begin() < vec.end());
    assert(vec.end() > vec.begin());

    // test insert and erase
    it = vec.begin() + 2;
    vec.insert(it, 10);
    assert(vec.size() == 6 && "iterator insert size failed");
    assert(*it == 10 && "iterator insert value failed");
    assert(*(it + 1) == 3 && "iterator insert position failed");

    it = vec.begin() + 2;
    vec.erase(it);
    assert(vec.size() == 5 && "iterator earse size failed");
    assert(*(vec.begin() + 3) == 4 && "iterator earse begin+2 failed");

    // test iterator range
    int expected = 1;
    for(const auto& elem : vec) {
        assert(elem == expected++ && "vec iterator value error");
    }
}

void testConstIterator()
{
    const wstl::vector<int> vec_const{1,2,3,4,5};

    // test iterator invalid
    assert(vec_const.begin() != vec_const.end() && "vec_const.begin() or vec_const.end() error");
    assert(*vec_const.begin() == 1 && "*begin() failed");
    assert(*(vec_const.end()-1) == 5 && "*end() failed");

    auto it = vec_const.begin();
    ++it;
    assert(*it == 2 && "it++ failed");

    it = vec_const.end();
    --it;
    assert(*it == 5 && "it--");

    assert(vec_const.begin() < vec_const.end());
    assert(vec_const.end() > vec_const.begin());

    // test iterator range
    int expected = 1;
    for(const auto& elem : vec_const) {
        assert(elem == expected++ && "vec_const iterator value error");
    }   
}

void testIterator()
{
    testIteratorDetail();
    testConstIterator();
    
    LOGI("vector iterator test passed!");
}

void testCapacity()
{
    wstl::vector<int> vec{1,2,3,4,5};
    assert(vec.empty() != true && "vec is empty");

    assert(vec.size() == 5 && "vec.size() error");
    assert(vec.max_size() > vec.capacity() && vec.capacity() > vec.size() && "vector store capablity error");

    vec.reserve(20);
    assert(vec.capacity() == 20 && "vector reserve failed");

    const wstl::vector<int> const_vec = vec;
    for(size_t i = 0; i < vec.size(); ++i) {
        assert(vec[i] == static_cast<int>(i+1) && vec.at(i) == static_cast<int>(i+1) && "vec opeartor[] or at() error");
        assert(const_vec[i] == static_cast<int>(i+1) && const_vec.at(i) == static_cast<int>(i+1) && "const_vec opeartor[] or at() error");
    }

    assert(vec.front() == 1 && const_vec.front() == 1 && "vector.front() failed");
    assert(vec.back() == 5 && const_vec.back() == 5 && "vector.back() failed");
    assert(*vec.data() == 1 && *const_vec.data() == 1 && "vector.data() failed");
    LOGI("vector capacity passed!");
}

void testAssign()
{
    wstl::vector<int> source{1,2,3,4,5};
    wstl::vector<int> vec;
    vec.assign(source.begin(), source.end());
    assert(vec.size() == source.size() && "vector assign iterator size error");
    for(size_t i = 0; i < vec.size(); ++i) {
        assert(vec[i] == source[i] && "vector assign iterator value error");
    }


    wstl::vector<int> vec1;
    vec1.assign(5,42);
    assert(vec1.size() == 5 && "vector assign (size and value) size error");
    for(size_t i = 0; i < vec1.size(); ++i) {
        assert(vec1[i] == 42 && "vector assign (size and value) value error");
    }

    wstl::vector<int> vec_list({1,2,3,4,5});
    for(size_t i = 0; i < vec_list.size(); ++i) {
        assert((vec_list[i]) == static_cast<int>(i+1) && "assign (initializer list) value error");
    }
    LOGI("vector assign passed!");
}

void testErase()
{
    wstl::vector<int> vec = {1,2,3,4,5};
    vec.erase(vec.begin() + 1, vec.begin() + 3);
    assert(vec.size() == 3 && "vector erase error");

    wstl::vector<int> vec_erase = {1,4,5};

    for(size_t i = 0; i < vec.size(); ++i) {
        assert(vec[i] == vec_erase[i] && "vector erase value error");
    }

    vec.erase(vec.begin() + 1);
    assert(vec.size() == 2 && "vector erase failed");

    vec.clear();
    assert(vec.empty());

    LOGI("vector erase test passed!");
}

void testPush()
{
    wstl::vector<TestClass> vec;
    vec.push_back(1);
    assert(vec.size() == 1 && "push back failed");
    assert(vec[0].getNum() == 1 && "push_back insert wrong value");

    vec.push_back(2);
    assert(vec.size() == 2 && "push_back failed");
    assert(vec[1].getNum() == 2 && "push_back inserted wrong value");

    LOGI("push back test passed!");
}

void testOperator()
{
    wstl::vector<TestClass> vec{TestClass(1), TestClass(2)};

    wstl::vector<TestClass> vec1(vec);
    assert(vec == vec1 && "vec is not equal vec1");

    wstl::vector<TestClass> vec2{TestClass(1), TestClass(3)};
    assert(vec < vec2 && "vec < vec2");
    assert(vec <= vec2 && "vec <= vec2");

    wstl::vector<TestClass> vec3{TestClass(1)};
    assert(vec != vec3 && "vec != vec3");

    assert(vec2 > vec1 && "vec2 > vec1");
    assert(vec2 >= vec1 && "vec2 >= vec1");

    LOGI("vector operator passed!");
}

int main()
{
    LOGI(DEBUG_DATE);
    testConstructor();
    testIterator();
    testCapacity();
    testAssign();
    testInsert();
    testEmplace();
    testPush();
    testResize();
    testErase();
    testOperator();
    return 0;
}