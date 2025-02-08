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


    LOGI("test queue constrcut passed!");

}

int main()
{
    testConstruct();
    return 0;
}