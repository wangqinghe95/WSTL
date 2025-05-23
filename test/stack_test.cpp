#include "wstack.hpp"

void testConstruct()
{
    wstl::stack<int> st;

    wstl::stack<int> st_n(10);
    assert(st_n.size() == 10 && st_n.top() == 0 && "stack(n) error");

    wstl::stack<int> st_n_value(10, 5);
    assert(st_n_value.size() == 10 && st_n_value.top() == 5 && "stack(n,value) error");

    wstl::deque<int> dq_n_value(10,5);
    wstl::stack<int> st_Iter(dq_n_value.begin(), dq_n_value.end());
    assert(st_Iter.size() == 10 && st_Iter.top() == 5 && "stack(Iter,Iter) error");

    std::initializer_list<int> ilist{1,2,3,4,5};
    wstl::stack<int> st_ilist(ilist.begin(), ilist.end());
    assert(st_ilist.size() == 5 && st_ilist.top() == 5 && "stack(ilist) error");
    
    wstl::stack<int> st_deque(dq_n_value);
    assert(st_deque.size() == 10 && st_deque.top() == 5 && "stack(deque) error");

    wstl::deque<int> dq_move_n_value(5,5);
    wstl::stack<int> st_container(wstl::move(dq_move_n_value));
    assert(st_container.size() == 5 && st_container.top() == 5 && dq_move_n_value.size() == 0 && "stack(Container&&) error");

    const wstl::stack<int> st_const_n_value(10,5);
    wstl::stack<int> st_stack(st_const_n_value);
    assert(st_stack.size() == 10 && st_stack.top() == 5 && "stack(const stack&) error");

    wstl::stack<int> st_move_1(5,10);
    wstl::stack<int> st_move(wstl::move(st_move_1));
    assert(st_move.size() == 5 && st_move.top() == 10 && st_move_1.empty() == true && "stack(stack&&) error");

    wstl::stack<int> st_operator;
    st_operator = st_move;
    assert(st_operator.size() == 5 && st_move.top() == 10 && "stack operator=() error");

    LOGI("test stack passed!");
}

void testPush()
{
    wstl::stack<int> st;
    st.push(5);
    assert(st.size() == 1 && st.top() == 5 && "push(value_type&& value) error");

    int val = 6;
    st.push(val);
    assert(st.size() == 2 && st.top() == 6 && "push(const value_type& value) error");

    LOGI("test stack push passed!");
}

void testEmplace()
{
    wstl::stack<int> st;
    st.emplace(5);
    assert(st.size() == 1 && st.top() == 5 && "emplace() error");

    st.pop();
    assert(st.empty() == true && "empty() error");

    LOGI("test stack emplace passed!");
}

int main()
{
    testConstruct();
    testPush();
    testEmplace();
    return 0;
}