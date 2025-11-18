#include <string>
#include <iostream>
#include <stdexcept>


using namespace std;

void test_f() {
    string s = "Hello, World!";
    try {
        s.at(100);
    } catch (out_of_range e) {
        cout << e.what() << endl;
    }
    return;
}


int main(int argc, char ** argv) {
    test_f();

    return 0;
}
