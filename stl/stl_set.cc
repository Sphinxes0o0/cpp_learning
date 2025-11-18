#include <cstdlib>
#include <iostream>
#include <set>
#include <random>

int main() {
    std::set<int> my_set;
    for (int i = 0; i < 10; ++i) {
        my_set.insert(i);
    }

    std::set<int>::iterator it;
    std::cout << "Elements in set: ";
    for (it = my_set.begin(); it != my_set.end(); ++it) {
        std::cout << *it << " ";
    }

    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<> dis(0, 100);
    std::cout << "\nRandomly generated numbers: " << dis(gen) << " " << dis(gen) << " " << dis(gen) << std::endl;

    my_set.insert(dis(gen));
    std::cout << "Elements in set after inserting a random number: ";
    for (it = my_set.begin(); it != my_set.end(); ++it) {
        std::cout << *it << " ";
    }


    std::multiset<int> my_multiset = {1, 2, 2, 3, 4, 5};
    std::cout << "\nSize of set: " << my_multiset.size() << std::endl;
    my_multiset.insert(2);
    my_multiset.insert(6);
    my_multiset.insert(2);
    for (auto v : my_multiset) {
        std::cout << v << " ";
    }

    std::cout << "\nCount of 2 in multiset: " << my_multiset.count(2) << std::endl;
    my_multiset.erase(2);
    for (auto v : my_multiset) {
        std::cout << v << " ";
    }
    std::cout << "count: \n" << my_multiset.count(2) << std::endl;

    std::cout << "find 2: " << (my_multiset.find(2) != my_multiset.end() ? "found" : "not found") << std::endl;
    std::cout << "find 5: " << (my_multiset.find(5) != my_multiset.end() ? "found" : "not found") << std::endl;

    return 0;
}
