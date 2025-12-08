#include <utility>
#include <cstdint>
#include <iostream>
// #include <print>

template<typename T>
class sPtr {
private:
    T *underlying_ptr;
    uint32_t *ref_Count;
public:
    void sPtrDelete() {
        delete underlying_ptr;
        delete ref_Count;
    }

    sPtr() : 
    underlying_ptr(nullptr), ref_Count(nullptr) {} 


    sPtr(T* newPtrImpl) {
        underlying_ptr = newPtrImpl;
        ref_Count = new uint32_t(1);
    }   

    sPtr(const sPtr<T>& other) {
        if(other.underlying_ptr != nullptr) {
            underlying_ptr = other.underlying_ptr;
            ref_Count = other.ref_Count;
            ++(*ref_Count); 
        } else {
            underlying_ptr = nullptr;
            ref_Count = nullptr;
        }
    }

    sPtr(sPtr<T>&& other) noexcept {
        if(other.underlying_ptr != nullptr) {
            underlying_ptr = other.underlying_ptr;
            ref_Count = other.ref_Count;

            other.ref_Count = nullptr;
            other.underlying_ptr = nullptr;
        }
    }
    

    sPtr& operator=(const sPtr<T>& other) {
        if((&other) == this) 
            return *this;
        // 1. Release current object:
        this->release();    
        
        // 2. Copy raw pointers from `other`
        if(other.underlying_ptr != nullptr) {
            underlying_ptr = other.underlying_ptr;
            ref_Count = other.ref_Count;
            ++(*ref_Count);
        }
        return *this;
    }


    sPtr& operator=(sPtr<T>&& other) {
        if((&other) == this) 
            return *this;

        this->release();

        if(other.underlying_ptr != nullptr) { 
            underlying_ptr = other.underlying_ptr;
            ref_Count = other.ref_Count;
 
            other.release();
        }
        return *this;
    }

    // Seems to be better
    sPtr& operator=(T* ptr) {
        if(underlying_ptr == ptr) {
            return *this;
        }
        // this->release();

        // underlying_ptr = ptr;
        // ref_Count = new uint32_t(1);
        this->reset(ptr);
        return *this;
    }

    sPtr& operator=(T&& val) {
        this->reset(new T(std::forward<T>(val)));   
        return *this;
    }

    T* get() const { return underlying_ptr; }


    void reset(T* ptr = nullptr) {
        if(underlying_ptr != ptr) {
            this->release();
        }
        underlying_ptr = ptr;
        if(ptr != nullptr) {
            ref_Count = new uint32_t(1);
        }
    }

    void release() {
        if(ref_Count != nullptr) {
            --(*ref_Count);
            if(*ref_Count == 0) {
                sPtrDelete();
            }
            ref_Count = nullptr;
            underlying_ptr = nullptr;
        }
    }

    uint32_t useCount() const { if(ref_Count == nullptr) return 0; else return (*ref_Count); }
    T& operator*() const { return *underlying_ptr; }
    T* operator->() const { return underlying_ptr; }

    ~sPtr() { release(); }
};

template<typename Q, typename ...QArguments>
    sPtr<Q> make_shared(QArguments&& ...args) {
        return sPtr<Q>(new Q(std::forward<QArguments>(args)...));
    }


        /*
    release():
What it SHOULD do:
Decrement the reference count of the current managed object.
Delete the object if the reference count reaches 0.
Optionally set the internal pointer to nullptr (depends on design).

What it SHOULD NOT do:
Dereference the pointer after releasing the object.
Automatically assign a new object.
Modify the reference counts of other smart pointers managing unrelated objects.

reset(T new_ptr = nullptr):*
What it SHOULD do:
Release the current object safely (call release()).
Take ownership of new_ptr.
Initialize the reference count to 1 for the new object.
Ensure the smart pointer remains in a valid state.

What it SHOULD NOT do:
Dereference the pointer before assigning a new object.
Leak the old object by skipping release().
Modify the reference counts of other smart pointers incorrectly.

Key points:
release() = safely let go of the current object.
reset() = safely replace the current object with a new one.
Avoid dereferencing the pointer after release or before reset.*/