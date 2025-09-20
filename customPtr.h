#include <type_traits>
#include <cstddef>

template<typename T>
class unique_ptr {
    T *ptr;
    public:
    unique_ptr(): ptr(nullptr){}
    unique_ptr(T *arg): ptr(arg){}
    unique_ptr(unique_ptr<T> &&arg) {
        if (ptr != nullptr) { delete ptr; }
        ptr = arg.ptr;
        arg.ptr = nullptr;
    }
    unique_ptr(unique_ptr<T> &arg) = delete;
    ~unique_ptr() {
        if (std::is_array<T>::value) {
            delete[] ptr;
        }
        else {
            delete ptr;
        }
    }
    T *get() {
        return ptr;
    }
    T &operator*() {
        return *ptr;
    }
    unique_ptr<T> &operator=(unique_ptr<T> &&arg) {
        ptr = arg.ptr;
        arg.ptr = nullptr;
        return *this;
    }
    unique_ptr<T> &operator=(unique_ptr<T> &arg) = delete;
};

template<typename T>
class unique_ptr<T[]> {
    T *ptr;
    public:
    unique_ptr(): ptr(nullptr){}
    unique_ptr(T *arg): ptr(arg){}
    unique_ptr(unique_ptr<T[]> &&arg) {
        ptr = arg.ptr;
        arg.ptr = nullptr;
    }
    unique_ptr(const unique_ptr<T> &arg) = delete;
    ~unique_ptr() {
            delete[] ptr;
    }
    T *get() {
        return ptr;
    }
    T &operator[](size_t index) {
        return *(ptr + index);
    }
    unique_ptr<T[]> &operator=(unique_ptr<T[]> &&arg) {
        if (ptr != nullptr) { delete[] ptr; }
        ptr = arg.ptr;
        arg.ptr = nullptr;
        return *this;
    }
    unique_ptr<T> &operator=(unique_ptr<T[]> &arg) = delete;
};

template<typename T>
class controlBlock {
    public:
    size_t shared_ptr_count, weak_ptr_count;
    T *ptr;
    controlBlock(): shared_ptr_count(1), weak_ptr_count(0), ptr(nullptr) {}
    controlBlock(T *arg): shared_ptr_count(1), weak_ptr_count(0), ptr(arg) {}
    void freePtr() {
        if (!--shared_ptr_count) {
            if (std::is_array<T>::value) {
                delete[] ptr;
            }
            else {
                delete ptr;
            }
        }
    }
};

template<typename T>
class controlBlock<T[]> {
    public:
    size_t shared_ptr_count, weak_ptr_count;
    T *ptr;
    controlBlock(): shared_ptr_count(1), weak_ptr_count(0), ptr(nullptr) {}
    controlBlock(T *arg): shared_ptr_count(1), weak_ptr_count(0), ptr(arg) {}
    void freePtr() {
        if (!--shared_ptr_count) {
            delete[] ptr;
        }
    }
};

template<typename T>
class shared_ptr {
    private:
    T *ptr;
    controlBlock<T> *ptrM;
    public:
    shared_ptr(): ptr(nullptr), ptrM(nullptr) {}
    shared_ptr(T *arg): ptr(arg), ptrM(new controlBlock<T>(arg)) {}
    shared_ptr(shared_ptr<T> &arg): ptr(nullptr), ptrM(nullptr) {
        if (&arg != this) {
            ptr = arg.ptr;
            ptrM = arg.ptrM;
            arg.ptrM->shared_ptr_count++;
        }
    }
    shared_ptr(shared_ptr<T> &&arg): ptr(arg.ptr), ptrM(arg.ptrM) {
        arg.ptr = nullptr;
        arg.ptrM = nullptr;
    }
    shared_ptr(T *arg1, controlBlock<T> *arg2): ptr(arg1), ptrM(arg2) {
        ptrM->shared_ptr_count++;
    }
    ~shared_ptr() {
        if (ptrM) {
            ptrM->freePtr();
            if (!ptrM->weak_ptr_count) {
                delete ptrM;
            }
        }
    }
    T &operator*() {
        return *ptr;
    }
    shared_ptr<T> &operator=(shared_ptr<T> &arg) {
        if (&arg != this) {
            ptr = arg.ptr;
            ptrM = arg.ptrM;
            arg.ptrM->shared_ptr_count++;
        }
        return *this;
    }
    shared_ptr<T> &operator=(shared_ptr<T> &&arg) {
        ptr = arg.ptr;
        ptrM = arg.ptrM;
        arg.ptr = nullptr;
        arg.ptrM = nullptr;
        return *this;
    }
    T *get() {
        return ptr;
    }
    controlBlock<T> *getControlBlock() {
        return ptrM;
    }
};

template<typename T>
class shared_ptr<T[]> {
    private:
    T *ptr;
    controlBlock<T[]> *ptrM;
    public:
    shared_ptr(): ptr(nullptr), ptrM(nullptr) {}
    shared_ptr(T *arg): ptr(arg), ptrM(new controlBlock<T[]>(arg)) {}
    shared_ptr(shared_ptr<T[]> &arg): ptr(nullptr), ptrM(nullptr) {
        if (&arg != this) {
            ptr = arg.ptr;
            ptrM = arg.ptrM;
            arg.ptrM->shared_ptr_count++;
        }
    }
    shared_ptr(shared_ptr<T[]> &&arg): ptr(arg.ptr), ptrM(arg.ptrM) {
        arg.ptr = nullptr;
        arg.ptrM = nullptr;
    }
    ~shared_ptr() {
        if (ptrM) {
            ptrM->freePtr();
            if (!ptrM->weak_ptr_count) {
                delete ptrM;
            }
        }
    }
    T &operator[](size_t index) {
        return *(ptr + index);
    }
    shared_ptr<T[]> &operator=(shared_ptr<T[]> &arg) {
        if (&arg != this) {
            ptr = arg.ptr;
            ptrM = arg.ptrM;
            arg.ptrM->shared_ptr_count++;
        }
        return *this;
    }
    shared_ptr<T[]> &operator=(shared_ptr<T[]> &&arg) {
        ptr = arg.ptr;
        ptrM = arg.ptrM;
        arg.ptr = nullptr;
        arg.ptrM = nullptr;
        return *this;
    }
    T *get() {
        return ptr;
    }
    controlBlock<T[]> *getControlBlock() {
        return ptrM;
    }
};

template<typename T>
class weak_ptr {
    private:
    T *ptr;
    controlBlock<T> *ptrM;
    public:
    controlBlock<T> *getControlBlock() {
        return ptrM;
    }
    weak_ptr(): ptr(nullptr), ptrM(nullptr) {}
    weak_ptr(T *arg) = delete;
    weak_ptr(weak_ptr<T> &arg): ptr(nullptr), ptrM(nullptr) {
        if (&arg != this) {
            ptrM = arg.getControlBlock();
            ptrM->weak_ptr_count++;
            ptr = ptrM->ptr;
        }
    }
    weak_ptr(weak_ptr<T> &&arg): ptr(arg.ptr, pltrM(arg.ptrM)) {
        arg.ptr = nullptr;
        arg.ptrM = nullptr;
    }
    weak_ptr(shared_ptr<T> &arg) {
        ptrM = arg.getControlBlock();
        ptrM->weak_ptr_count++;
        ptr = ptrM->ptr;
    }
    weak_ptr(shared_ptr<T> &&arg) = delete;
    ~weak_ptr() {
        if (ptrM) {
            if (!(ptrM->shared_ptr_count + 
            --ptrM->weak_ptr_count)) {
                delete ptrM;
            }
        }
    }
    weak_ptr<T> &operator=(T *arg) = delete;
    weak_ptr<T> &operator=(weak_ptr<T> &arg) {
        if (&arg != this) {
            ptrM = arg.getControlBlock();
            ptrM->weak_ptr_count++;
            ptr = ptrM->ptr;
        }
        return *this;
    }
    weak_ptr<T> &operator=(weak_ptr<T> &&arg) {
        ptr = arg.ptr;
        ptrM = arg.ptrM;
        arg.ptr = nullptr;
        arg.ptrM = nullptr;
        return *this;
    }
    weak_ptr<T> &operator=(shared_ptr<T> &arg) {
        if (&arg != this) {
            ptrM = arg.getControlBlock();
            ptrM->weak_ptr_count++;
            ptr = ptrM->ptr;
        }
        return *this;
    }
    weak_ptr<T> &operator=(shared_ptr<T> &&arg) = delete;
    shared_ptr<T> lock() {
        return shared_ptr<T>(ptr, ptrM);
    }
};

template<typename T>
class weak_ptr<T[]> {
    private:
    T *ptr;
    controlBlock<T[]> *ptrM;
    public:
    controlBlock<T[]> *getControlBlock() {
        return ptrM;
    }
    weak_ptr(): ptr(nullptr), ptrM(nullptr) {}
    weak_ptr(T *arg) = delete;
    weak_ptr(weak_ptr<T[]> &arg): ptr(nullptr), ptrM(nullptr) {
        if (&arg != this) {
            ptrM = arg.getControlBlock();
            ptrM->weak_ptr_count++;
            ptr = ptrM->ptr;
        }
    }
    weak_ptr(weak_ptr<T[]> &&arg): ptr(arg.ptr, pltrM(arg.ptrM)) {
        arg.ptr = nullptr;
        arg.ptrM = nullptr;
    }
    weak_ptr(shared_ptr<T[]> &arg) {
        ptrM = arg.getControlBlock();
        ptrM->weak_ptr_count++;
        ptr = ptrM->ptr;
    }
    weak_ptr(shared_ptr<T[]> &&arg) = delete;
    ~weak_ptr() {
        if (ptrM) {
            if (!(ptrM->shared_ptr_count + 
            --ptrM->weak_ptr_count)) {
                delete ptrM;
            }
        }
    }
    weak_ptr<T[]> &operator=(T *arg) = delete;
    weak_ptr<T[]> &operator=(weak_ptr<T[]> &arg) {
        if (&arg != this) {
            ptrM = arg.getControlBlock();
            ptrM->weak_ptr_count++;
            ptr = ptrM->ptr;
        }
        return *this;
    }
    weak_ptr<T[]> &operator=(weak_ptr<T[]> &&arg) {
        ptr = arg.ptr;
        ptrM = arg.ptrM;
        arg.ptr = nullptr;
        arg.ptrM = nullptr;
        return *this;
    }
    weak_ptr<T[]> &operator=(shared_ptr<T[]> &arg) {
        if (&arg != this) {
            ptrM = arg.getControlBlock();
            ptrM->weak_ptr_count++;
            ptr = ptrM->ptr;
        }
        return *this;
    }
    weak_ptr<T[]> &operator=(shared_ptr<T[]> &&arg) = delete;
    shared_ptr<T[]> lock() {
        return shared_ptr<T[]>(ptr, ptrM);
    }
};

template<typename T>
typename std::remove_reference<T>::type &&move(T &&arg) {
    return static_cast<typename std::remove_reference<T>::type>(arg);
}