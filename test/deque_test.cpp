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
    /** if the following two variables are defined before use
     *  it will result in a double free memory error **
    */
    // wstl::deque<int> dp_assign, dp_assign_right_value;
    wstl::deque<int> dp_assign = dp_original;
    assert(dp_assign.size() == 5 && dp_assign[0] == 1 && dp_assign[4] == 5 && "operator=(deque&) error");

    wstl::deque<int> dp_assign_right_value = wstl::move(dp_original);
    assert(dp_assign_right_value.size() == 5 && dp_assign_right_value[0] == 1 && dp_assign_right_value[4] == 5 && dp_original.size() == 0 && "operator=(deque&&) error");

    LOGI("test construct passed!");
}


void testPushAndEmplace()
{
    wstl::deque<int> emplace_test_deque{1,2,3,4,5};
    emplace_test_deque.emplace_front(0);
    assert(emplace_test_deque.size() == 6 && emplace_test_deque[0] == 0 && "emplace front error");

    emplace_test_deque.emplace_back(6);
    assert(emplace_test_deque.size() == 7 && emplace_test_deque.back() == 6 && "emplace back error");

    wstl::deque<int> push_test_deque{1,2,3,4,5};
    push_test_deque.push_back(6);
    assert(push_test_deque.size() == 6 && push_test_deque.back() == 6 && "push back error");

    push_test_deque.push_front(7);
    assert(push_test_deque.size() == 7 && push_test_deque.front() == 7 && "push front error");

    LOGI("test emplace/front passed!");
}


void testClear()
{
    wstl::deque<int> deque_test{1,2,3,4};
    deque_test.clear();
    assert(deque_test.size() == 0 && "deque clear() error");

    LOGI("test clear passed!");

}

void testMove()
{
    wstl::deque<int> dp_assign_right_value;
    wstl::deque<int> dp_original = {1,2,3,4,5};
    dp_assign_right_value = wstl::move(dp_original);
    assert(dp_assign_right_value.size() == 5 && dp_assign_right_value[0] == 1 && dp_assign_right_value[4] == 5 && dp_original.size() == 0 && "operator=(deque&&) error");

    LOGI("test move passed!");
}

void testPop()
{
    wstl::deque<int> dq_pop {1,2,3,4,5};
    dq_pop.pop_back();
    assert(dq_pop.size() == 4 && dq_pop.back() == 4 && "deque pop_back() error");

    dq_pop.pop_front();
    assert(dq_pop.size() == 3 && dq_pop.front() == 2 && "deque pop_front() error");

    LOGI("test pop passed!");
}

void testInsert()
{
    wstl::deque<int> dq_insert {1,2,3,4};
    int val = 5;
    dq_insert.insert(dq_insert.begin()+3, val);
    assert(dq_insert.size() == 5 && dq_insert[3] == 5 && "deque insert(iterator, value)");

    dq_insert.insert(dq_insert.begin()+3, 6);
    assert(dq_insert.size() == 6 && dq_insert[3] == 6 && "deque insert(iterator, value_type&&)");

    dq_insert.insert(dq_insert.begin(), 3, val);
    assert(dq_insert.size() == 9 && dq_insert[0] == 5 && "deque insert(iterator, n, value)");

    wstl::deque<int> dq_insert_copy;
    dq_insert_copy.insert(dq_insert_copy.begin(), dq_insert.begin(), dq_insert.end());
    assert(dq_insert_copy.size() == 9 && dq_insert.front() == 5 && dq_insert.back() == 4 && "deque insert(iterator, n, value)");

    LOGI("test insert passed!");
}

void testErase()
{
    wstl::deque<int> dq_erase{1,2,3,4,5};
    dq_erase.erase(dq_erase.begin()+2);
    assert(dq_erase.size() == 4 && "deque erase(iterator)");

    dq_erase.erase(dq_erase.begin(), dq_erase.end());
    assert(dq_erase.size() == 0 && "deque erase(iterator, iterator)");


    LOGI("test erase passed!");
}

void testSwap()
{
    wstl::deque<int> dq_swap_1 {1,2,3,4,5};
    wstl::deque<int> dq_swap_2 {6,7,8,9};

    dq_swap_1.swap(dq_swap_2);
    assert(dq_swap_1.size() == 4 && dq_swap_2.size() == 5 && "deque swap error");

    LOGI("test swap passed!");
}

int main()
{
    testConstruct();
    testPushAndEmplace();
    testPop();
    testClear();
    testInsert();
    testErase();
    testSwap();
    return 0;
}