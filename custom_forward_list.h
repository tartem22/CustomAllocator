#ifndef CUSTOM_FORWARD_LIST_H
#define CUSTOM_FORWARD_LIST_H

#include <memory>

template <typename T>
class ForwardListElement
{
public:
    ForwardListElement(T *data, ForwardListElement<T> *ptr = nullptr)
        : _data(data), _ptr_to_next(ptr)
    {
    }

    T *_data = nullptr;
    ForwardListElement<T> *_ptr_to_next = nullptr;
};

template <typename T, typename A = std::allocator<T>>
class CustomForwardList
{
public:
    CustomForwardList()
    {
        _allocator = new A();
    }

    CustomForwardList& operator =(std::initializer_list<T> list)
    {
        for(auto &l : list)
            emplace_front(l);
        return *this;
    }

    void emplace_front(const T val)
    {
        T *ptr = _allocator->allocate(1);
        if (_size == 0)
        {
            _allocator->construct(ptr, val);
            ForwardListElement<T> *beginForwardListElement =
                new ForwardListElement<T>(ptr, nullptr);
            _begin = beginForwardListElement;

            T *ptr_end = _allocator->allocate(1);
            ForwardListElement<T> *endForwardListElement =
                new ForwardListElement<T>(ptr_end, nullptr);
            _begin->_ptr_to_next = endForwardListElement;
            _end = endForwardListElement;
        }
        else
        {
            _allocator->construct(_end->_data, val);

            ForwardListElement<T> *endForwardListElement =
                new ForwardListElement<T>(ptr, nullptr);
            _end->_ptr_to_next = endForwardListElement;
            _end = endForwardListElement;
        }
        _size++;
    }

    ~CustomForwardList()
    {
        ForwardListElement<T> *elementToDelete = nullptr;
        for (auto it = this->begin(); it != this->end();)
        {
            _allocator->destroy(it->_data);
            _allocator->deallocate(it->_data, 1);
            elementToDelete = it.get();
            it = it++;
            delete elementToDelete;
        }
        delete _allocator;
    }

    T front()
    {
        return *(_begin->_data);
    }

    auto begin()
    {
        return CustomIterator(_begin);
    }

    auto end()
    {
        if (_size == 0)
            return CustomIterator(_begin);
        else
            return CustomIterator(_end);
    }

    struct CustomIterator : std::iterator<std::forward_iterator_tag, const CustomForwardList>
    {
    public:
        explicit CustomIterator(ForwardListElement<T> *first) : current(first)
        {
        }

        CustomIterator operator++(int)
        {
            CustomIterator tmp(current->_ptr_to_next);
            return tmp;
        }

        CustomIterator &operator++()
        {
            current = current->_ptr_to_next;
            return *this;
        }

        bool operator==(const CustomIterator &it) const
        {
            return current == it.current;
        }
        bool operator!=(const CustomIterator &it) const
        {
            return !(*this == it);
        }

        auto operator->()
        {
            return current;
        }

        auto &operator*()
        {
            return *(current->_data);
        }
        const ForwardListElement<T> &operator*() const
        {
            return *(current->_data);
        }

        auto get()
        {
            return current;
        }

    private:
        ForwardListElement<T> *current;
    };

private:
    A *_allocator = nullptr;
    ForwardListElement<T> *_begin = nullptr;
    ForwardListElement<T> *_end = nullptr;
    size_t _size = 0;
};

#endif