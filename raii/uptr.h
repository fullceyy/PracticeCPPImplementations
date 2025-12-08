#include <iostream>

template<typename T>
class uPtr {
private:
    T* underlying_ptr;
public:
    ~uPtr() { uPtrDelete(); }
    uPtr() : underlying_ptr(nullptr) {}
    // copying not allowed, will not be unique
    uPtr(const uPtr<T>& other) = delete;
    uPtr& operator=(const uPtr<T>& other) = delete;
    // transfer ownership
    uPtr(uPtr<T>&& other) noexcept {                
        if(other.underlying_ptr != nullptr) {
            underlying_ptr = other.underlying_ptr;
            // other.underlying_ptr = nullptr;
            other.release();
        }
        else {
            std::cout << "moving nullptr!\n";
        }
    }
    uPtr& operator=(uPtr<T>&& other) noexcept {
        if(&other == this) return *this;
        
        if(other.underlying_ptr != nullptr) {
            underlying_ptr = other.underlying_ptr;
            other.release();
        }
        return *this;
    }
    // raw pointer conversion
    // should this hold data by copying it and getting rid of rawPtr
    // should this just take pointer?
    uPtr(T* rawPtr) {
        if(rawPtr != nullptr) {
            reset(rawPtr);
        }
    }
    uPtr& operator=(T* rawPtr) {
        if(rawPtr != nullptr) {
            reset(rawPtr);
        }
    }


    T& operator*() const { return *underlying_ptr; }
    T* operator->() const { return underlying_ptr; }
    T* get() const { return underlying_ptr; }
    
    T* release() {
        if(underlying_ptr != nullptr) {
            underlying_ptr = nullptr;
        }
        return underlying_ptr;
    }

    void reset(T* newRawPtr) {
        if(newRawPtr == underlying_ptr) return;
        auto oldPtr = get();
        uPtrDelete();

    }

    void swap(uPtr<T>& other) {
        std::swap(underlying_ptr, other.underlying_ptr);
    }

    void uPtrDelete() {
        if(underlying_ptr != nullptr) {
            delete underlying_ptr;
        }
    }
};

template<typename Q, typename ...QArgs>
uPtr<Q> make_unique(QArgs&& ...args) {
    return uPtr<Q>(new Q(std::forward<QArgs>(args)...));
}