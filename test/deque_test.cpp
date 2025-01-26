#include "test_common.hpp"
#include "wdeque.hpp"

void testConstruct()
{
    wstl::deque<int> dq;
    assert(dq.size() == 0 && "deque() error");
    // LOGI("max_size() = ", dq.max_size());

    wstl::deque<int> dq_size(10);
    assert(dq_size.size() == 10 && dq_size[0] == 0 && dq_size[9] == 0 && "deque(n) error");

    wstl::deque<int> dp_size_value(10,5);
    assert(dp_size_value.size() == 10 && dp_size_value[0] == 5 && dp_size_value[9] == 5 && "deque(n,value) error");

    int arr[] = {1,2,3,4,5};
    wstl::deque<int> dp_copy(arr, arr+5);
    assert(dp_copy.size() == 5 && dp_copy[0] == 1 && dp_copy[4] == 5 && "deque(iterator,iterator) error");

    wstl::deque<int> dp_list {1,2,3,4,5};
    assert(dp_list.size() == 5 && dp_list[0] == 1 && dp_list[4] == 5 && "deque(list) error");

    wstl::deque<int> dp_right_value_copy(dp_copy);
    assert(dp_right_value_copy.size() == 5 && dp_right_value_copy[0] == 1 && dp_right_value_copy[4] == 5 && "deque(deque&) error");

    wstl::deque<int> dp_right_reference(wstl::move(dp_list));
    assert(dp_right_reference.size() == 5 && dp_right_reference[0] == 1 && dp_right_reference[4] == 5 && dp_list.size() == 0 && "deque(deque&&) error");

    wstl::deque<int> dp_original = {1,2,3,4,5};
    wstl::deque<int> dp_assign, dp_assign_right_value;
    dp_assign = dp_original;
    assert(dp_assign.size() == 5 && dp_assign[0] == 1 && dp_assign[4] == 5 && "operator=(deque&) error");

    dp_assign_right_value = wstl::move(dp_original);
    LOGI(dp_assign_right_value.size());
    LOGI(dp_assign_right_value[0]);
    LOGI(dp_assign_right_value[4]);
    LOGI(dp_original.size());
    assert(dp_assign_right_value.size() == 5 && dp_assign_right_value[0] == 1 && dp_assign_right_value[4] == 5 && dp_original.size() == 0 && "operator=(deque&&) error");


    LOGI("deque construct passed!");
}

int main()
{
    testConstruct();
    return 0;
}