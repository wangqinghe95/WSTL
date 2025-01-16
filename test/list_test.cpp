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

    list_test.swap(list_test_1);
    assert(list_test.size() == 6 && (list_test.back()) == 2 && list_test_1.size() == 6 && (list_test_1.back()) == 0 && "list swap error");

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

void testPushAndPop()
{
    wstl::list<int> list_test{1,2,3,4};
    list_test.push_back(5);
    assert(list_test.size() == 5 && list_test.back() == 5 && "list_test push_back(value&) error");

    list_test.push_front(6);
    assert(list_test.size() == 6 && list_test.front() == 6 && "list_test push_front(value&) error");

    wstl::list<std::string> list_string;
    list_string.push_back(std::string("hello"));
    assert(list_string.size() == 1 && list_string.front() == "hello" && "list_string push_back(value&&) error");

    list_test.pop_back();
    assert(list_test.size() == 5 && list_test.back() == 4 && "list_test pop_back() error");

    list_test.pop_front();
    assert(list_test.size() == 4 && list_test.front() == 1 && "list_test pop_front() error");


    LOGI("test push/pop passed!");
}

void testSplice()
{
    wstl::list<int> list1 = {1,2,3};
    wstl::list<int> list2 = {4,5,6};

    list1.splice(list1.end(), list2);
    assert(list1.size() == 6 && list1.back() == 6 && "list splice(pos,list&) error");

    list2 = {7,8,9};
    auto it = list1.begin();
    wstl::advance(it, 2);
    list1.splice(it, list2);
    assert(list1.size() == 9 && list1.back() == 6 && "list splice(pos,list&) error");

    list1 = {1,2,3,4,5,6};
    list2 = {10,20,30};

    it = wstl::find(list2.begin(), list2.end(), 20);
    if(it != list2.end()) {
        list1.splice(list1.end(), list2, it);
    }

    auto it2 = wstl::find(list1.begin(), list1.end(), 20);
    assert(it2 != list1.end() && "list splice(pos, list&, value) error");

    list1 = {1,2,3,4,5,6};
    list2 = {10,20,30};

    auto it_first = wstl::find(list1.begin(), list1.end(), 3);
    auto it_second = wstl::find(list1.begin(), list1.end(), 5);
    
    if(it_first != list1.end() && it_second != list1.end()) {
        list2.splice(list2.begin(), list1, it_first, it_second);
    }
    assert(list2.size() == 5 && list2.front() == 3 && "list splice(pos, list&, iter1, iter2) error");

    LOGI("test splice passed!");
}

void testRemove()
{
    wstl::list<int> list_test = {1,2,3,4,5};
    list_test.remove(2);
    auto it = wstl::find(list_test.begin(), list_test.end(), 2);
    assert(it == list_test.end() && "list_test remove() error");

    auto f = [](int val){
        return val % 2 == 0;
    };
    list_test.remove_if(f);
    assert(list_test.size() == 3 && "list_test remove_if() error");

    LOGI("test remove passed!");
}

void testUnique()
{
    wstl::list<int> list_test = {1,2,3,3,4,4,4,5,5,5,5};
    list_test.unique();
    assert(list_test.size() == 5 && "list_test unique() error");

    // wstl::list<int> list_test = {3, 6, 9, 12, 1, 4, 7, 10, 13, 2, 5, 8, 11, 14};

    // test unique(BinaryPredicate pred)

    LOGI("test unique passed!");
}

void testMerge()
{
    wstl::list<int> list_test_1 = {1,2,4,6,7};
    wstl::list<int> list_test_2 = {3,5,8};

    list_test_2.merge(list_test_1);

    assert(list_test_2.size() == 8 && list_test_2.front() == 1 && "list merge(list&) error");

    // list_test_2.clear();
    wstl::list<int> list_test_1_1 = {1,2,4,6,7};
    wstl::list<int> list_test_2_1 = {3,5,8};

    list_test_2_1.merge(list_test_1_1, [](const int& x, const int& y){
        return x > y;
    });

    // list_test_2.merge(list_test_1, wstl::greater<int>());
    // list_test_2_1.merge(list_test_1_1, wstl::less<int>());

    for(auto it = list_test_2_1.begin(); it != list_test_2_1.end(); ++it) {
        LOGI(*it);
    }
    assert(list_test_2_1.size() == 8 && list_test_2_1.front() == 8 && "list merge(list&, comp) error");
    
    
    LOGI("test merge passed!");

}

int main()
{
    testConstrucotr();
    testClear();
    testResize();
    testAssign();
    testEmplace();
    testPushAndPop();
    testSplice();
    testRemove();
    testUnique();
    testMerge();
    return 0;
}