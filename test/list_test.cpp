#include "wlist.hpp"
#include "test_common.hpp"
#include "wvector.hpp"

void testConstrucotr()
{
    wstl::list<int> w_list;
    assert(w_list.empty() == true && "w_list empty and construct error");

    wstl::list<int> w_list_n(5);
    assert(w_list_n.size() == 5 && "w_list_n constructed failed!");

    wstl::list<TestClass> w_class_list;
    assert(w_class_list.empty() == true && "w_class_list empty and construct error");

    wstl::list<TestClass> w_class_list_n(4);
    assert(w_class_list_n.size() == 4 && "w_class_list_n constructed failed!");

    wstl::list<TestClass> w_class_list_n_value(3,5);
    assert(w_class_list_n_value.size() == 3 && *w_class_list_n_value.begin() == 5  && "w_class_list_n_value constructed failed!");

    wstl::vector<int> vec{1,2,3,4,5};
    wstl::list<int> lst(vec.begin(), vec.end());
    assert(lst.size() == 5 && *lst.begin() == 1  && "lst constructed failed!");

    wstl::list<int> initializer_list{1,2,3,4,5};
    assert(initializer_list.size() == 5 && *lst.begin() == 1  && "initializer_list constructed failed!");

    wstl::list<int> copy_list(initializer_list);
    assert(copy_list.size() == 5 && *copy_list.begin() == 1  && "copy_list constructed failed!");

    wstl::list<int> move_list(std::move(initializer_list));
    assert(move_list.size() == 5 && initializer_list.size() == 0 && "move_list constructor failed!");

    wstl::list<int> list_operator_equal;
    list_operator_equal = move_list;
    assert(list_operator_equal.size() == 5 && *list_operator_equal.begin() == 1  && "list_operator_equal constructed failed!");

    LOGI("test wlist constructor passed!");
}

void testClear()
{
    wstl::list<int> my_list = {1,2,3,4,5};
    wstl::list<int> my_list_copy = my_list;

    for(auto it = my_list.begin(); my_list.end() != it; it++) {
        // LOGI(*it);
    }

    // LOGI("my_list.begin():", *my_list.begin() );
    my_list.erase(my_list.begin());
    // LOGI("my_list.begin():", *my_list.begin() );
    // LOGI("my_list.size():", my_list.size() );
    assert(*my_list.begin()  == 2 && "my_list erase error");

    my_list.erase(my_list.begin(), my_list.end());
    assert(my_list.size() == 0 && "my_list erase all error");

    my_list_copy.clear();
    assert(my_list_copy.size() == 0 && "my_list_copy clear error");

    LOGI("test clear passed!");
}

void testResize()
{
    wstl::list<int> list_test{1,2,3,4,5};
    assert(list_test.size() == 5 && "list_size error");

    list_test.resize(4);
    assert(list_test.size() == 4 && "list resize narrow error");

    list_test.resize(6);
    assert(list_test.size() == 6 && (list_test.back()) == 0 && "list resize large error");

    wstl::list<int> list_test_1{1,2,3,4,5};
    list_test_1.resize(4, 1);

    assert(list_test_1.size() == 4 && (list_test_1.back()) == 4 && "list resize narrow error");

    list_test_1.resize(6, 2);
    assert(list_test_1.size() == 6 && (list_test_1.back()) == 2 && "list resize large with number error");

    LOGI("test resize passed!");
}

void testAssign()
{
    wstl::list<int> list_test;
    assert(list_test.size() == 0 && "list_size error");
    list_test.assign(5, 1);
    assert (list_test.size() == 5 && (list_test.back()) == 1 && "list assign(number,value) error");
    
    wstl::list<int> list_test_1{6,7,8,9,10};
    wstl::list<int> list_test_2;
    list_test_2.assign(list_test_1.begin(), list_test_1.end());
    assert(list_test_2.size() == 5 && (list_test_2.back()) == 10 && "list assign(iterator) error");

    LOGI("test assign passed!");
}

void testEmplace()
{
    wstl::list<int> list_test {1,2,3,4,5};
    list_test.emplace(list_test.begin(), 6);
    assert(list_test.size() == 6 && *list_test.begin() == 6 && "list emplace begin() error");

    list_test.emplace_back(7);
    assert(list_test.size() == 7 && list_test.back() == 7 && "list emplace_back() error");

    list_test.emplace_front(8);
    assert(list_test.size() == 8 && *list_test.begin() == 8 && "list emplace_front() error");

    LOGI("test emplace passed!");
}

void testPush()
{
    
}

int main()
{
    testConstrucotr();
    testClear();
    testResize();
    testAssign();
    testEmplace();
    testPush();
    return 0;
}