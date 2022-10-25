#include "Test.hpp"
#include <iostream>
#include "CircBuf.hpp"
using namespace std;

Test::Test() {
    CircBuf <int> history;
    cout << history.size() << endl;
    cout << history.push_front(2) << endl;
    cout << history.front() << endl;
    cout << history.accumulate() << endl;
}