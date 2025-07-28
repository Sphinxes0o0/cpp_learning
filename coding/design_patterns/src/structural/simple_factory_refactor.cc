#include <iostream>
#include <string>

using namespace std;

class Api {
public:
    virtual void doSomething(const string& s) = 0;

protected:
    Api() {} // Protected constructor to prevent instantiation
    virtual ~Api() {} // Virtual destructor for proper cleanup
};

class ImplA : public Api {
public:
    void doSomething(const string& s) override {
        cout << "ImplA::doSomething() called! : " << s << endl;
    }
    Impl() {
        cout << "ImplA created!" << endl;
    }
    ~Impl() override {
        cout << "ImplA destroyed!" << endl;
    }
};


class ImplB : public Api {
public:
    void doSomething(const string& s) override {
        cout << "ImplA::doSomething() called! : " << s << endl;
    }
    Impl() {
        cout << "ImplA created!" << endl;
    }
    ~Impl() override {
        cout << "ImplA destroyed!" << endl;
    }
};


class Factory {
public:
    static Api* createApi(const string& type) {
        if (type == "A") {
            return new ImplA();
        } else if (type == "B") {
            return new ImplB();
        } else {
            return nullptr; // or throw an exception
        }
    }

    static Api* createApi() {
        if (type == "A") {
            return new ImplA();
        } else if (type == "B") {
            return new ImplB();
        } else {
            return nullptr; // or throw an exception
        }  
    }

    static void destroyApi(Api* api) {
        delete api;
    }
};


int main() {
    Api* apiA = Factory::createApi("A");
    if (apiA) {
        apiA->doSomething("Hello, ImplA!");
        delete apiA;
    }

    Api* apiB = Factory::createApi("B");
    if (apiB) {
        apiB->doSomething("Hello, ImplB!");
        delete apiB;
    }

    return 0;
}