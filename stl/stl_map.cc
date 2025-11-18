#include <map>
#include <iostream>


int main() {
    std::map<int, std::string> my_map;
    my_map[1] = "one";
    my_map[2] = "two";
    my_map[3] = "three";
    my_map[4] = "four";

    std::cout << "Elements in map:\n";
    for (const auto& pair : my_map) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }
    my_map.insert({5, "five"});
    my_map.insert(std::make_pair(6, "six"));
    for (const auto& pair : my_map) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    std::multimap<std::string, int> my_multimap;
    my_multimap.insert({"apple", 1});
    my_multimap.insert({"banana", 2});
    my_multimap.insert({"apple", 3});
    my_multimap.insert({"orange", 4});
    my_multimap.insert({"banana", 5});
    std::cout << "\nElements in multimap:\n";
    for (const auto& pair : my_multimap) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }
    std::cout << "Count of 'apple': " << my_multimap.count("apple") << std::endl;
    my_multimap.erase("apple");
    std::cout << "Elements in multimap after erasing 'apple':\n";
    for (const auto& pair : my_multimap) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }


    return 0;
}
