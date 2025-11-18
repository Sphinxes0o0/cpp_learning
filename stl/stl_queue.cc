#include <queue>
#include <list>
#include <iostream>

int main() {
    std::queue<int> q;
    for (int i = 0; i < 10; ++i) {
        q.push(i);
    }
    std::queue<int> q_copy(q); // Copy the queue
    std::cout << "Elements in queue: ";
    while (!q_copy.empty()) {
        std::cout << q_copy.front() << " ";
        q_copy.pop();
    }

    std::cout << std::endl;

    std::list<int> list_data;
    list_data.push_back(1);
    list_data.push_back(2);
    list_data.push_back(3);
    list_data.push_back(4);
    list_data.push_back(5);
    std::cout <<"list data size " << list_data.size() << std::endl;

    std::queue<int, std::list<int> > q_list(list_data); // Initialize queue with list
    while (!q_list.empty()) {
        std::cout << q_list.front() << " ";
        q_list.pop();
    }

    std::list<int> list_data1(5, 5);
    std::cout << "\nElements in list_data1: ";
    for (std::list<int>::iterator it = list_data1.begin(); it != list_data1.end(); ++it) {
        std::cout << *it << " ";
    }

    list_data1.push_back(6);
    list_data1.push_front(4);
    std::cout << "\nElements in list_data1 after push_back(6) and push_front(4): ";
    for (std::list<int>::iterator it = list_data1.begin(); it != list_data1.end(); ++it) {
        std::cout << *it << " ";
    }

    std::list<int> list_data2(1, 10);
    std::cout << "\nElements in list_data2: ";
    for (std::list<int>::iterator it = list_data2.begin(); it != list_data2.end(); ++it) {
        std::cout << *it << " ";
    }

    list_data2.push_back(11);
    std::cout << "\nElements in list_data2 after push_back(11): ";
    for (std::list<int>::iterator it = list_data2.begin(); it != list_data2.end(); ++it) {
        std::cout << *it << " ";
    }

    list_data2.insert(list_data2.end(), 12);


    return 0;
}
