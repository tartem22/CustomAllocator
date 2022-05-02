#ifndef CUSTOM_ALLOCATOR
#define CUSTOM_ALLOCATOR

#include <iostream>
#include <cstdlib>

template <typename T, std::size_t S = 10>
struct CustomAllocator
{
    using value_type = T;

    using pointer = T *;
    using const_pointer = const T *;
    using reference = T &;
    using const_reference = const T &;

    template <typename U>
    struct rebind
    {
        using other = CustomAllocator<U, S>;
    };

    CustomAllocator()
    {
        _memory_parts.reserve(10);
    }
    ~CustomAllocator()
    {
        for(auto &mem : _memory_parts)
            std::free(mem);
    }

    template <typename U>
    CustomAllocator(const CustomAllocator<U, S> &)
    {
        _memory_parts.reserve(10);
    }

    T *allocate(std::size_t n)
    {
        if (_buffer_ptr == nullptr)
        {
            if (n > _reserve_size)
                _reserve_size = n;
            // std::cout << "allocate: [n = " << _reserve_size << "]" << std::endl;
            _buffer_ptr = reinterpret_cast<T *>(std::malloc(_reserve_size * sizeof(T)));
            if (!_buffer_ptr)
                throw std::bad_alloc();
            else
            {
                _memory_parts.emplace_back(_buffer_ptr);
                _data_ptr = _buffer_ptr;
                _curr_data_ptr = _data_ptr + n;
            }
        }
        else
        {
            if (_curr_data_ptr + n < _buffer_ptr + _reserve_size)
            {
                _data_ptr = _curr_data_ptr;
                _curr_data_ptr = _data_ptr + n;
            }
            else
            {
                _buffer_ptr = nullptr;
                allocate(n);
            }
        }
        return _data_ptr;
    }

    void deallocate(T *p, std::size_t n)
    {
        // std::cout << "deallocate: [n  = " << n << " from " << p << "] " << std::endl;
    }

    template <typename U, typename... Args>
    void construct(U *p, Args &&...args)
    {
        new (p) U(std::forward<Args>(args)...);
    }

    void destroy(T *p)
    {
        p->~T();
    }

private:
    std::size_t _reserve_size = S;
    T *_buffer_ptr = nullptr;
    T *_data_ptr = nullptr;
    T *_curr_data_ptr = nullptr;
    std::vector<T*> _memory_parts;
};

#endif