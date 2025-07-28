#include <iostream>
#include <string>

using namespace std;

class Api {
public:
    virtual void doSomething(string s) = 0;
    virtual ~Api() {}

protected:
    Api() {} // Protected constructor to prevent instantiation

};


class Impl : public Api {
public:
    void doSomething(string s) override {
        cout << "Impl::doSomething() called! : " << s << endl;
    }

    Impl() {
        cout << "Impl created!" << endl;
    }

    ~Impl() override {}
};

int main() {
    Api* api = new Impl();
    api->doSomething("Hello, Simple Factory!");
    delete api;
    return 0;
}