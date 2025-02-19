#include "wqueue.hpp"
#include "wvector.hpp"

void testConstruct()
{
    wstl::queue<int> qe_test(10);
    assert(qe_test.size() == 10 && "queue(n) error");

    wstl::queue<int> qe_test_n_value(10,10);
    assert(qe_test_n_value.size() == 10 && qe_test_n_value.front() == 10 && qe_test_n_value.back() == 10 && "queue(n,value) error");

    wstl::vector<int> vec{1,2,3,4,5};
    wstl::queue<int> qe_iter(vec.begin(), vec.end());
    assert(qe_iter.size() == 5 && qe_iter.front() == 1 && "queue(IIter, IIter) error");

    wstl::queue<int> qe_initializer{1,2,3,4,5};
    assert(qe_initializer.size() == 5 && qe_initializer.front() == 1 && "queue(std::initializer_list) error");

    wstl::deque<int> deque_test{1,2,3,4,5};
    wstl::queue<int> qe_container(deque_test);
    assert(qe_container.size() == 5 && qe_container.front() == 1 && "queue(Container&) error");

    wstl::deque<int> queue_orig {1,2,3,4,5};
    wstl::queue<int> queue_move(wstl::move(queue_orig));
    assert(queue_move.size() == 5 && queue_move.front() == 1 && "queue(Container&&) error");

    wstl::queue<int> queue_orig_1 {1,2,3,4,5};
    wstl::queue<int> queue_copy(queue_orig_1);
    assert(queue_copy.size() == 5 && queue_copy.front() == 1 && "queue(const queue&) error");

    wstl::queue<int> queue_orig_2 {1,2,3,4,5};
    wstl::queue<int> queue_copy_1(wstl::move(queue_orig_2));
    assert(queue_copy_1.size() == 5 && queue_copy_1.front() == 1 && "queue(queue&&) error");

    wstl::queue<int> queue_operator;
    queue_operator = queue_copy_1;
    assert(queue_operator.size() == 5 && queue_operator.front() == 1 && "operator=(const queue&) error");
    
    wstl::queue<int> queue_operator_2;
    queue_operator_2 = wstl::move(queue_copy_1);
    assert(queue_operator_2.size() == 5 && queue_operator_2.front() == 1 && "operator=(queue&&) error");

    wstl::queue<int> queue_operator3;
    queue_operator3 = {1,2,3,4,5};
    assert(queue_operator3.size() == 5 && queue_operator3.front() == 1 && "operator=(initializer_list) error");

    LOGI("test queue constrcut passed!");

}

void testEmplace()
{
    wstl::queue<int> queue_emplace {1,2,3,4,5};
    queue_emplace.emplace(6);
    assert(queue_emplace.size() == 6 && queue_emplace.back() == 6 && "queue emplace(Args&&) error");

    int val = 7;
    queue_emplace.push(val);
    assert(queue_emplace.size() == 7 && queue_emplace.back() == 7 && "queue push(const value_type&) error");

    queue_emplace.push(wstl::move(8));
    assert(queue_emplace.size() == 8 && queue_emplace.back() == 8 && "queue push(value_type&&) error");

    queue_emplace.pop();
    assert(queue_emplace.size() == 7 && queue_emplace.front() == 2 && "queue pop() error");

    queue_emplace.clear();
    assert(queue_emplace.size() == 0 && "queue clear()");

    LOGI("test emplace passed!");
}

void testSwap()
{
    wstl::queue<int> queue_1{1,2,3,4,5};
    wstl::queue<int> queue_2{6,7,8,9};
    queue_1.swap(queue_2);
    assert(queue_1.size() == 4 && queue_2.size() == 5 && "queue swap error");

    LOGI("queue swap passed!");
}

void testOperator()
{
    wstl::queue<int> queue_1 {1,2,3,4,5};
    wstl::queue<int> queue_2 (queue_1);

    assert(queue_1 == queue_2 && "queue operator= error");

    wstl::queue<int> queue_3 {1,2,3,4,5,6};
    assert(queue_1 < queue_3 && "queue operator< error");

    LOGI("test operator passed!");
}

int testQueue()
{
    testConstruct();
    testEmplace();
    testSwap();
    testOperator();
    return 0;
}

void testPriorityQueueConstruct()
{
    wstl::priority_queue<int> p_queue;

    wstl::priority_queue<int> p_queue_n(10);
    assert(p_queue_n.size() == 10 && "priority_queue(n) error");

    wstl::priority_queue<int> p_queue_n_value(10, 10);
    assert(p_queue_n_value.size() == 10 && p_queue_n_value.top() == 10 && "priority_queue(n,value) error");

    int arr[] = {6,1,2,3,4,5};
    wstl::priority_queue<int> p_queue_iter(arr, arr+6);
    // wstl::priority_queue<int,wstl::vector<int>,wstl::greater<int>> p_queue_iter(arr, arr+6);
    
    // LOGI(p_queue_iter.top());
    assert(p_queue_iter.size() == 6 && p_queue_iter.top() == 6 && "priority_queue(iter,iter) error");

    wstl::priority_queue<int> p_queue_list{5,1,2,3,9};
    assert(p_queue_list.size() == 5 && p_queue_list.top() == 9 && "priority_queue(list) error");
    
    const wstl::vector<int> vec{5,2,6,8,9};
    wstl::priority_queue<int> p_queue_container(vec);
    assert(p_queue_container.size() == 5 && p_queue_container.top() == 9 && "priority_queue(Container&) error");

    wstl::vector<int> vec1{5,2,6,8,9};
    wstl::priority_queue<int> p_queue_container_move(wstl::move(vec1));
    assert(vec1.size() == 0 && p_queue_container_move.size() == 5 && p_queue_container_move.top() == 9 && "priority_queue(Container&&) error");

    wstl::priority_queue<int> p_queue_move {5,1,2,7,9};
    wstl::priority_queue<int> p_queue_copy(wstl::move(p_queue_move));
    assert(p_queue_move.size() == 0 && p_queue_copy.size() == 5 && p_queue_copy.top() == 9 && "priority_queue(priority_queue&&) error");

    wstl::priority_queue<int> p_operator;
    p_operator = p_queue_copy;
    assert(p_operator.size() == p_queue_copy.size() && p_operator.size() == 5 && p_operator.top() == 9 && "operator= error");

    wstl::priority_queue<int> p_operator_ori_1 {1,2,3,4,5};
    wstl::priority_queue<int> p_operator_move_1;
    p_operator_move_1 = wstl::move(p_operator_ori_1);
    assert(p_operator_move_1.size() == 5 && p_operator_move_1.top() == 5 && "operator=(priority_queue&&) error");

    wstl::priority_queue<int> p_operator_2;
    p_operator_2 = {4,2,1,7,8,9};
    assert(p_operator_2.size() == 6 && p_operator_2.top() == 9 && "operator=(std::initializer_list) error");

    LOGI("test priority queue passed!");
}

void testPriorityQueueEmplace()
{
    wstl::priority_queue<int> pri_que {2,4,5,8,9};
    assert(pri_que.size() == 5 && pri_que.top() == 9 && "priority_queue error");

    pri_que.emplace(10);
    assert(pri_que.size() == 6 && pri_que.top() == 10 && "priority_queue emplace error");

    pri_que.pop();
    assert(pri_que.size() == 5 && pri_que.top() == 9 && "priority_queue pop error");

    pri_que.push(10);
    assert(pri_que.size() == 6 && pri_que.top() == 10 && "priority_queue push(value_type&&) error");

    const int value = 11;
    pri_que.push(value);
    assert(pri_que.size() == 7 && pri_que.top() == 11 && "priority_queue push(const value_type&) error");

    LOGI("test priority_queue emplace passed!");

}

void testPriorityQueue()
{
    testPriorityQueueConstruct();
    testPriorityQueueEmplace();
}

int main()
{
    testPriorityQueue();
    return 0;
}