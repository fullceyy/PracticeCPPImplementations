#include <iostream>
#include "sptr.h"

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
// new tests;
void test_move_assign_correctness();
void test_self_move_assign();
void test_double_release();
void test_copy_from_null();
void test_self_copy_assign();
void test_raw_assign_leak();
void test_rvalue_value_assign();
void test_refcount_many_scopes();
void test_dtor_exactly_once();
void test_many_copies();

int main(void) {
    // test_basic();
    // test_copy();
    // test_copy_assign();
    // test_move();
    // test_move_assign();
    // test_refcount_lifetime();
    // test_assign_raw();
    // test_null_behavior();
    // test_make_shared();
    // test_operators();

    // new tests;
    test_move_assign_correctness();
    test_self_move_assign();
    test_double_release();
    {
        std::cout << "Hello world!" << std::endl;
    }
    test_copy_from_null();
    test_self_copy_assign();
    test_raw_assign_leak();
    test_rvalue_value_assign();
    test_refcount_many_scopes();
    test_dtor_exactly_once();
    test_many_copies();
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

// new tests
void test_move_assign_correctness() {
    sPtr<int> a(new int(10));
    sPtr<int> b(new int(20));

    b = std::move(a);  // should NOT delete the underlying int(10)

    std::cout << "b = " << *b << "\n";        // EXPECT: 10
    std::cout << "a.get() = " << a.get() << "\n"; // EXPECT: nullptr
}

void test_self_move_assign() {
    sPtr<int> a(new int(99));

    a = std::move(a);  // MUST be safe

    std::cout << "self move: " << *a << "\n";  // EXPECT: 99
    std::cout << "count: " << a.useCount() << "\n"; // EXPECT: 1
}

void test_double_release() {
    sPtr<int> a(new int(5));
    a.release();       // should delete
    a.release();       // should do nothing
}

void test_copy_from_null() {
    sPtr<int> a;  // null
    sPtr<int> b(a);

    std::cout << "b.get=" << b.get() << "\n"; // EXPECT: nullptr
}

void test_self_copy_assign() {
    sPtr<int> a(new int(7));
    a = a;

    std::cout << "self copy: " << *a << "\n"; // EXPECT: 7
}

void test_raw_assign_leak() {
    sPtr<int> a(new int(1));
    a = new int(2);   // must delete previous int(1)

    std::cout << *a << "\n";  // EXPECT: 2
}

void test_rvalue_value_assign() {
    sPtr<int> a(new int(5));
    a = 10;       // EXPECT: *a becomes 10
                  // REAL: UB, because operator=(const T&&) is broken
    std::cout << *a << "\n";
}

void test_refcount_many_scopes() {
    sPtr<int> a(new int(3));

    {
        sPtr<int> b(a);
        {
            sPtr<int> c(b);
            std::cout << a.useCount() << "\n"; // EXPECT: 3
        }
        std::cout << a.useCount() << "\n";     // EXPECT: 2
    }
    std::cout << a.useCount() << "\n";         // EXPECT: 1
}

void test_dtor_exactly_once() {
    sPtr<int> a(new int(10));
    {
        sPtr<int> b(a);
        sPtr<int> c(a);
    }
    // Only now should the last one be deleted.
    std::cout << a.useCount() << "\n"; // EXPECT: 1
}

void test_many_copies() {
    sPtr<int> p(new int(123));

    for(int i=0; i<1000; i++) {
        sPtr<int> q(p);
    }

    std::cout << p.useCount() << "\n";  // EXPECT: 1
}
