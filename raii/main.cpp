#include <iostream>
#include "sharedptr.h"

#define pr() std::cout << "DMSG" << std::endl;

void test_basic();
void test_copy();
void test_copy_assign();
void test_move();
void test_move_assign();
void test_refcount_lifetime();
void test_assign_raw();
void test_null_behavior();
void test_make_shared();
void test_operators();

int main(void) {
    test_basic();
    test_copy();
    test_copy_assign();
    test_move();
    test_move_assign();
    test_refcount_lifetime();
    test_assign_raw();
    test_null_behavior();
    test_make_shared();
    test_operators();
}

void test_basic() {
    sPtr<int> p(new int(10));
    std::cout << *p << "\n";            // EXPECT: 10
    std::cout << p.useCount() << "\n";  // EXPECT: 1
}

void test_copy() {
    sPtr<int> a(new int(5));
    sPtr<int> b(a);

    std::cout << *a << " " << *b << "\n";     // EXPECT: 5 5
    std::cout << a.useCount() << "\n";        // EXPECT: 2
    std::cout << b.useCount() << "\n";        // EXPECT: 2
}

void test_copy_assign() {
    sPtr<int> a(new int(7));
    sPtr<int> b(new int(9));
    sPtr<int> c(new int(5));
    std::cout << "Init a, b, c\n";
    b = a;
    c = a;
    
    std::cout << *b << "\n";            // EXPECT: 7
    std::cout << b.useCount() << "\n";  // EXPECT: 2
    std::cout << *c << "\n";
}

void test_move() {
    sPtr<int> a(new int(42));
    sPtr<int> b(std::move(a));

    std::cout << *b << "\n";            // EXPECT: 42
    std::cout << a.get() << "\n";
    // EXPECT: "a" is empty/nullptr
}

void test_move_assign() {
    sPtr<int> a(new int(100));
    // pr();
    sPtr<int> b;
    b = std::move(a); // breaks the whole thing, test tomorrow<>
    // move assignment is incorrectly written
    // release was deleting ;)
    // pr();
    std::cout << *b << "\n";            // EXPECT: 100
    std::cout << a.get() << "\n";
    // EXPECT: a is nullptr
}

void test_refcount_lifetime() {
    sPtr<int> a(new int(4));
    {
        sPtr<int> b(a);
        std::cout << a.useCount() << "\n";   // EXPECT: 2
    }
    std::cout << a.useCount() << "\n";       // EXPECT: 1
}

void test_assign_raw() {
    sPtr<int> a;
    a = new int(33);

    std::cout << *a << "\n";            // EXPECT: 33
    std::cout << a.useCount() << "\n";  // EXPECT: 1
}

void test_null_behavior() {
    sPtr<int> a;
    std::cout << "null get: " << a.get() << "\n"; // EXPECT: nullptr

    sPtr<int> b(a);
    // EXPECT: does not crash and does not print warning
}

void test_make_shared() {
    auto p = make_shared<int>(52);
    std::cout << *p << "\n";            // EXPECT: 52
    std::cout << p.useCount() << "\n";  // EXPECT: 1
}

void test_operators() {
    sPtr<int> p(new int(10));
    
    int x = *p;       // EXPECT: 10
    int y = p.get()[0]; // EXPECT: 10

    std::cout << x << " " << y << "\n";
}