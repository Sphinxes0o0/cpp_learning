#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

void display(int x) {
    std::cout << x << " ";
}

bool f(int x) {
    bool is_even = (x % 2 == 0);
    cout << "Checking " << x << " -> "
         << (is_even ? "even" : "odd") << "\n";
    return is_even;
}

void test_f() {
    vector<int> v = {1, 2, 3, 4, 5, 6};
    cout << count_if(v.begin(), v.end(), f);
}

void test_00() {
    vector<int> v;
    int x;
    cin >> x;
    while (x > 0) {
        v.push_back(x);
        cin >> x;
    }

    cout << "Max = " << *max_element(v.begin(), v.end()) << std::endl;
    cout << "Sum = " << accumulate(v.begin(), v.end(), 0) << std::endl;

    sort(v.begin(), v.end());
    for_each(v.begin(), v.end(), display);
    cout << std::endl;
}



int main() {

    test_f();

    return 0;
}
