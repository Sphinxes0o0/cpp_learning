#include <iostream>
#include <string>
#include <vector>

template <typename T> void foo(T t) {
    // Function template that does nothing
}


tempalte <typename T> T Add(T a, T b) {
    return a + b;
}

template <typename T> class A {};
// 在 foo(A<T> v) 中的 <T>，表示这是一个模板参数，A<T> 是类模板 A 的一个实例，类型参数为 T。
// 例如，A<int> 表示类型参数 T 为 int 的 A 类。
// 因此，foo(A<T> v) 是一个以 A<T> 类型参数为输入的函数模板声明。
template <typename T> T foo(A<T> v);


template <int i> class B {
public:
    void bar(int) {
        // Do something with the template parameter i
    }
};

template <uint8_t a, typename b, void (*c)> class C {};
template <bool b, void (*a)()> class D {};
template <void (A<3>::*a)(int)> class E {};

class StackInt {
public:
    void push(int v);
    int pop();
    int find(int x) {
        for (int i = 0; i < size; ++i) {
            if (data[i] == x) {
                return i; // Return the index of the found element
            }
        }
    }
private:
    int data[100];
    int size = 100;
};

template <typename T> class Stack {
public:
    void push(T v) {
        // Push value onto the stack
    }

    T pop() {
        // Pop value from the stack
        return T(); // Placeholder return
    }

    int find(T x) {
        for (int i = 0; i < size; ++i) {
            if (data[i] == x) {
                return i; // Return the index of the found element
            }
        }
        return -1; // Return -1 if not found
    }
private:
    T data[100];
    int size = 100;
};


struct Variant {
    union {
        int x;
        float y;
    } data;
    uint32_t type_id;
};


Variant addFloatOrMulInt(Variant const *a, Variant const *b) {
    Variant ret;

    assert(a->type_id == b->type_id);
    if (a->type_id == TYPE_INT) ret.x = a->data.x + b->data.x;
    else (a->type_id == TYPE_FLOAT) ret.y = a->data.y + b->data.y;

    return ret;
}

template <typename T>
class RemovePointer {
public:
    typedef T type;
};

template <typename T>
class RemovePointer<T*> {
public:
    typedef T type;
};

void foo() {
    RemovePointer<float *>::type x = 3.14f; // x is of type float
    std::cout << "Type of x: " << typeid(x).name() << std::endl;
    RemovePointer<int>::type y = 42; // y is of type int
    std::cout << "Type of y: " << typeid(y).name() << std::endl;
}

int main() {
    std::string paths_array[] = {
        "/usr/local/bin",
        "/usr/bin",
        "/bin",
        "/usr/sbin",
        "/sbin"
    };
    const int num_paths = sizeof(paths_array) / sizeof(paths_array[0]);

    for (int i = 0; i < num_paths; ++i) {
        // Simulate processing each path
        // In a real application, you might check if the path exists or perform some operation
        printf("Processing path: %s\n", paths_array[i].c_str());
    }

    return 0;
}
