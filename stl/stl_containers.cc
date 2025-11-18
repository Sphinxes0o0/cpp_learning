#include <iostream>
#include <list>

using namespace std;


int main(int argc, char** argv) {
    int m, n;
    cout << "Enter number of elements in first list: ";
    cin >> n >> m;

    list<int> children;
    for (int i=0; i < n; i++) children.push_back(i);

    list<int>::iterator current = children.begin();

    while (children.size() > 1) {
        for (int i = 0;  i < m; i++) {
            current++;
            if (current == children.end()) {
                current = children.begin();
            }
            current = children.erase(current);
            if (current == children.end()) {
                current = children.begin();
            }
        }
    }
    cout << "Last remaining child: " << *current << endl;

    return 0;
}
