#include <cstdio>
#include <iostream>
#include <map>
#include <utility>

int main() {

    std::pair<int, int> p(1, 2);
    std::printf("p.first = %d, p.second = %d\n", p.first, p.second);
    std::swap(p.first, p.second);
    std::printf("p.first = %d, p.second = %d\n", p.first, p.second);

    std::map<std::string, int> table;
    table["a"] = 1;

    std::cout << table["b"] << std::endl;

    //打印map
    for (auto &item : table) {
        std::cout << item.first << " " << item.second << std::endl;
    }

    return p.first + p.second;
}