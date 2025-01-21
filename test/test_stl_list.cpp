#include <iostream>
#include <list>

using namespace std;

void testSTLListMerge()
{
    list<int> list_1 = {1,2,4,6,7};
    list<int> list_2 = {3,5,8};
    list_1.merge(list_2, [](const int& x, const int& y){
        return x > y;
    });

    for(auto& item : list_1) {
        cout << item << endl;
    }
}

int main()
{
    testSTLListMerge();
    return 0;
}