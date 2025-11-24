#include <utility>
#include <print>

template<typename T>
class sPtr {
private:
    T *underlying_ptr;
    uint32_t *ref_Count;
public:
    sPtr() : 
    underlying_ptr(nullptr), ref_Count(nullptr) {}
    // maybe check somehow whether the pointed address already has a sharedPtr looking at it, meaning we can just get the refCount of the memory address and put it here, reduce double deletion?
    sPtr(T* newPtrImpl) : 
    underlying_ptr(newPtrImpl), ref_Count(new uint32_t(0)) {}    
    
    sPtr(const sPtr<T>& other) {
        if(other.underlying_ptr != nullptr) {
            underlying_ptr = other.underlying_ptr;
            ref_Count = other.ref_Count;
            ++(*ref_Count); 
        } else {
            std::println("copy constructor value has nullptr as u_ptr.");
        }
    }

    sPtr(const sPtr<T>&& other) :
    underlying_ptr(other.underlying_ptr), ref_Count(other.ref_Count) {
        if(other.underlying_ptr != nullptr) {
            ++(*ref_Count);
        }
    }

    sPtr& operator=(const sPtr<T>& other) {
        if((&other) == this)
            return *this;
        
        --(*ref_Count);
        underlying_ptr = other.underlying_ptr;
        ref_Count = other.ref_Count;
        
            if(other.underlying_ptr != nullptr) {
            sPtrDelete(other);
            // OR
            // other.sPtrDeleteNoComposition();
            ++(*ref_Count);
        }
    }

    sPtr& operator=(const sPtr<T>&& other) {
        if((&other) == this) 
            return *this;

        --(*ref_Count);
        if(*ref_Count == 0) { sPtrDelete(other); }
        std::exchange(underlying_ptr, other.underlying_ptr);
        // underlying_ptr = other.underlying_ptr;
        ref_Count = other.ref_Count;
        sPtrDelete(other);
    }

    // This is questionable;
    sPtr& operator=(const T&& TVal) {
        if(underlying_ptr == TVal) { return *this; }
        *underlying_ptr = TVal; // is this even okay syntax wise?
        if(ref_Count == nullptr) 
        { ref_Count(new uint32_t(0)); }
    }

    void sPtrDeleteNoComposition() {
        if(underlying_ptr == nullptr) { 
            std::println(
                "Underlying Pointer of this shared_ptr instance is already nullptr."
            );
            return; 
        }

        delete underlying_ptr;
        if((*ref_Count) >= 0) {
            delete ref_Count;
        }
    }

    void sPtrDelete(const sPtr<T>& to_Delete) {
        if(to_Delete.underlying_ptr == nullptr) { 
            std::println(
                "Underlying Pointer of this shared_ptr instance is already nullptr."
            );
            return; 
        }

        delete to_Delete.underlying_ptr;
        if(to_Delete.(*ref_Count) >= 0) {
            delete to_Delete.ref_Count;
        }
    }

    // should they all handle nullptr situations?
    T* get() const { return underlying_ptr; }
    uint32_t useCount() const { return (*ref_Count); }
    T& operator*() const { return *underlying_ptr; }
    T& operator->() const { return underlying_ptr; }

    // make_shared
    template<typename Q, typename ...QArguments>
    Q* make_shared(QArguments&& ...args) {
        return sPtr<Q>(new Q(std::forward<args>(args)...));
    }

    // destructor
    ~sPtr() { sPtrDelete(this); }
    // if the underlying ptr is nullptr then the refCount is not initialized;

        /* TODO:
       1. Implement destructor to decrement ref_Count and delete underlying_ptr if last owner.
       2. Fix copy constructor to increment other.ref_Count correctly (and handle nullptr safely).
       3. Complete copy assignment operator:
           - handle self-assignment
           - decrement current ref_Count and delete if needed
           - copy underlying_ptr and ref_Count from other
           - increment new ref_Count safely
       4. Implement move constructor:
           - steal underlying_ptr and ref_Count from other
           - set other’s pointers to nullptr
       5. Implement move assignment operator:
           - handle self-assignment
           - decrement current ref_Count and delete if needed
           - steal pointers from other
           - set other’s pointers to nullptr
       6. Optional: implement reset(T* newPtr = nullptr)
       7. Optional: implement use_count() to return strong reference count
       8. Optional: overload operator*() and operator->() to access underlying object safely
    */
};