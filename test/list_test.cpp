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

    for(auto it = my_list.begin(); my_list.end() != it; it++) {
        LOGI(*it);
    }

    LOGI("my_list.begin():", *my_list.begin() );
    my_list.erase(my_list.begin());
    LOGI("my_list.begin():", *my_list.begin() );
    LOGI("my_list.size():", my_list.size() );
    assert(*my_list.begin()  == 2 && "my_list erase error");

    LOGI("test clear passed!");
}

int main()
{
    testConstrucotr();
    testClear();
    return 0;
}