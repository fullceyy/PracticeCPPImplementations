#include <utility>

template<typename T>
class sPtr {
private:
    T *underlying_ptr;
    uint32_t *ref_Count;
public:
    sPtr() : underlying_ptr(nullptr), ref_Count(nullptr) {}
    sPtr(T* newPtrImpl) : underlying_ptr(newPtrImpl), ref_Count(new uint32_t(1)) {}    
    sPtr(const sPtr<T>& other) : 
    underlying_ptr(other.underlying_ptr), ref_Count(ref_Count) {
        *(ref_Count)++; 
    }

    sPtr& operator=(const sPtr<T>& other) {
        if((&other) == this)
            return *this;
    }
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