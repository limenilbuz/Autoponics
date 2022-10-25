#include "Test.hpp"
#include <iostream>
#include "CircBuf.hpp"
using namespace std;

Test::Test() {
    CircBuf <int> history;
    cout << history.size() << endl;
    history.push_front(2);
    cout << history.front() << endl;
    cout << history.accumulate() << endl;
}