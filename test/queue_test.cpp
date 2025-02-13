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

int main()
{
    testConstruct();
    testEmplace();
    testSwap();
    testOperator();
    return 0;
}