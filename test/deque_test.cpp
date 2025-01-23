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

    LOGI("deque construct passed!");
}

int main()
{
    testConstruct();
    return 0;
}