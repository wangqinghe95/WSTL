#include "wlist.hpp"
#include "test_common.hpp"

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

    LOGI("test wlist constructor passed!");
}

int main()
{
    testConstrucotr();
    return 0;
}