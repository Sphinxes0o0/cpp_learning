#include <deque>
#include <iostream>
#include <string>

int main() {
    std::deque<int> d;
    std::deque<int>::iterator it;
    for (int i = 0; i < 10; ++i) {
        d.push_back(i);
    }
    std::cout << "Elements in deque: ";
    for (it = d.begin(); it != d.end(); ++it) {
        std::cout << *it << " ";
    }

    std::deque<int> d_9(10, 9);
    std::deque<int> d_10(d_9);
    std::cout << "\nElements in d_9: ";
    for (auto it : d_9) {
        std::cout << it << " ";
    }

    std::cout << "\nElements in d_10: ";
    for (auto it : d_10) {
        std::cout << it << " ";
    }

    d_10.push_back(11);
    std::cout << "\nElements in d_10 after push_back(11): " << d_10.back() << std::endl;

    d_10.pop_back();
    std::cout << "Elements in d_10 after pop_back(): " << d_10.back() << std::endl;


    std::deque<std::string> d_str;
    d_str.push_back("Hello");
    d_str.push_back("World");
    std::cout << "Elements in d_str: " << d_str.front() << " " << d_str.back() << std::endl;

    d_str.clear();

    return 0;
}
