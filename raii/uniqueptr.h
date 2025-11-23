#include <utility>

namespace CC {
    template<typename T>
    class u_ptr {
    private:
        T* underlying_ptr {};
    public:
        u_ptr() { }
        u_ptr(const u_ptr<T>&) = delete; // copy constructor
        u_ptr(u_ptr<T>&& other) : underlying_ptr(nullptr) {
            std::swap(underlying_ptr, other.underlying_ptr);
        } // move constructor
        u_ptr& operator=(const u_ptr<T>&) = delete; // copy assignment

        u_ptr& operator=(const u_ptr<T>&& other) {
            if(*this == other) {
                return (*this);
            }

            delete_underlying_ptr();
            underlying_ptr = std::exchange(other.underlying_ptr, nullptr);
            return (*this);
        } // move assignment

        // deleted for unique ownership
        ~u_ptr() { // destructor;
            delete_underlying_ptr();
        }

        u_ptr(T* raw) : underlying_ptr(raw) {  }

        // TODO :
        // Initialize from raw Pointer of type T!
        /* swap(unique_ptr&, unique_ptr&) — non-member version
        (Optional) make_unique<T>(...) — helper factory function
        Partial specialization for arrays: unique_ptr<T[]>
        Support for custom deleters (store deleter inside the class)
        Ownership transfer: move constructor / move assignment only */

        T* get() const noexcept {
            return underlying_ptr;
        }

        /* since u_ptr is just a wrapper for a raw pointer, at any point we can just release that pointer, no longer making it unique! */
        T* release() {
            return std::exchange(underlying_ptr, nullptr);
        }

        /* this guy on the other hand, resets the owner of type T pointer, so it make T* ptr a new unique_ptr, while other one is not. */
        void reset(T* ptr) {
            if(underlying_ptr != ptr) {
                delete_underlying_ptr();
                underlying_ptr = ptr; // ??
            } else { return; }

            // auto old_underlying_ptr = get();
            // if(old_underlying_ptr) {
            //     delete_underlying_ptr();
            //     old_underlying_ptr = ptr;
            // }

            // if(underlying_ptr == ptr) {
            //     return;
            // }
            // delete_underlying_ptr();
            // std::swap(underlying_ptr, ptr);
        }


        /* I believe swap requires pointer of the same type, meaning you can only swap to u_ptr wrapped raw pointers, that's why it takes this param. */
        // maybe It should take 2 params?
        void swap(CC::u_ptr<T>& ptr) noexcept {
            std::swap(*this.ptr, ptr.underlying_ptr);
        }

        T& operator*() const {
            return (*underlying_ptr); // dereferenced right? // why braces are safer?
        }

        T* operator->() const {
            return underlying_ptr; // just returns pointer to access it's members
        }

        // if nullptr then false, otherwise true?
        // it was just return u_ptr before;
        // now it's u_ptr != nullptr; if u_ptr not equal to nullptr then true; otherwise false;
        operator bool() const noexcept { return underlying_ptr != nullptr; }

        template<typename Q, typename ...Args>
        Q* make_unique(Args&& ... args) {
            return u_ptr<Q>(new Q(std::forward<Args>(args)...));
        }


        void delete_underlying_ptr() {
            if(underlying_ptr != nullptr) {
                delete underlying_ptr;
                underlying_ptr = nullptr;
            }
        }
    };
};
