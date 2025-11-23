#include <iostream>
#include <memory>
#include <ostream>
#include <vector>

class Interface {
public:
    virtual ~Interface() = default;
    virtual std::unique_ptr<Interface> objectClone() const = 0;
    virtual void ObjectName() const = 0;

};

template<typename Derived>
class BaseImplementation : public Interface {
protected:
    BaseImplementation() = default;
public:
    virtual void ObjectName() const override {
        std::cout << static_cast<const Derived&>(*this).name << std::endl;
    }

    virtual std::unique_ptr<Interface> objectClone() const override {
        /* Explaining this static_cast
         * so what happens in static_cast<const T& or alt. T const &>(*this);
         *
         * this in our case is `BaseImplementation<Derived> *`
         *
         * if we do just static_cast<Derived>(this);
         * we cast this(pointer to calling object) to a Derived OBJECT, invalid conversion, compiler cries;
         *
         * if we do static_cast<Derived&>(this);
         * we cast `this` to a reference of Derived type, invalid conversion;
         * this is a pointer and Derived is reference type, can't do ptr -> ref;
         *
         * if we do static_cast<Derived*>(this);
         * we cast `this` to a pointer of derived* which is valid;
         * BaseImplementation<ActualDerivedOne> ADO;
         * ADO is then casted to ActualDerivedOne * type (downcasting), only safe in CRTP,
         * or the cases where you know where you are casting it;
         *
         * if we do static_cast<Derived*>(*this);
         * casting from derferenced `this`, meaning we get BaseImplementation<Derived>& to Derived *;
         * invalid conversion; object -> pointer not allowed;
         *
         * if we do static_cast<Derived>(*this);
         * casting dereferenced `this` to a newly crated Derived object, loss of memory;
         *
         * if we do static_cast<Derived&>(*this);
         * casting deref. `this` to a Derived reference, no slices, no data lost;
         *
         */

        return std::make_unique<Derived>(static_cast<Derived const &>(*this));
    }

    friend std::ostream& operator<<(std::ostream& stream, const BaseImplementation<Derived>& object) {
        stream << "Obj address : " << &object;
        return stream;
    }
};


class ActualDerivedOne : public BaseImplementation<ActualDerivedOne> {
public:
    friend BaseImplementation<ActualDerivedOne>;
    ActualDerivedOne() : x(3), y(2), z(1), name("ONE") {
        std::cout << x << " " << y << " " << z << std::endl;
    }
private:
    int x, y, z;
    std::string name;
    // void impl() {
    //     std::cout << "Impl of ActualDerived was called!\n";
    // }
};

class ActualDerivedTwo : public BaseImplementation<ActualDerivedTwo> {
public:
    friend BaseImplementation<ActualDerivedTwo>;
    ActualDerivedTwo() : x(1), y(2), name("TWO") {
                std::cout << x << " " << y << std::endl;
    }
private:
    int x, y;
    std::string name;
};

int main() {
    // BaseImplementation<ActualDerived> bi;
    // bi.callADimpl();
    std::vector<ActualDerivedOne*> adoPtrs;
    // adoPtrs.resize(10);
    for(int q = 0; q < 10; q++) { adoPtrs.push_back(new ActualDerivedOne); }
    for(auto& s : adoPtrs) { s->objectClone(); std::cout << s << std::endl; }
    // std::vector<ActualDerivedTwo*> adtPtrs;

    std::vector<Interface*> anyKindPtrs;
    int s = 3;
    // int w = 5;
    for(int q = 0; q < s; q++) {
        if(q == 1) { anyKindPtrs.push_back(new ActualDerivedOne); continue; }
        anyKindPtrs.push_back(new ActualDerivedTwo);
    }

    for(auto& s : anyKindPtrs) { s->ObjectName(); }
    // std::cout << "hey\n";

    return 0;
}
