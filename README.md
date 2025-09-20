# Custom Smart Pointer Library

This project implements C++ smart pointers from scratch:

- `unique_ptr<T>` and `unique_ptr<T[]>`
- `shared_ptr<T>` and `shared_ptr<T[]>`
- `weak_ptr<T>` and `weak_ptr<T[]>`
- `move()` utility (replacement for `std::move`)

✅ Supports full array types (`T[N]`)  
✅ Automatically chooses between `delete` and `delete[]`  
✅ Safe control block management for `shared_ptr` and `weak_ptr`  
✅ No STL dependency required

This implementation demonstrates full ownership semantics and safe memory handling with both single objects and dynamically allocated arrays.