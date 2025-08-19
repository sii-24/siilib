#include <memory>

#include "Exception.hpp"
#include "OneLinkedList.cpp"


namespace siilib {
template <typename T, typename Container = OneLinkedList<T>>
class Queue {

    Container c;
    size_t max_length{0};

public:
    Queue(size_t max_length=0) : max_length(max_length) { }
    Queue(const Queue<T>& right) : max_length(right.max_length), c(right.c) { }
    Queue(Queue<T>&& right) noexcept : max_length(right.max_length), c(std::move(right.c)) { }

    void clear() { c.clear(); }

    bool is_empty() const { return c.get_length() == 0; }

    size_t get_length() const { return c.get_length(); }


    T& push(const T& x) { 
        if(max_length) if(c.get_length() >= max_length) throw OverflowError();
        return c.push_back(x);
    }
    T& push(T&& x) {
        if(max_length) if(c.get_length() >= max_length) throw OverflowError();
        return c.push_back(std::move(x));
    }

    T pop() { 
        if(c.get_length() == 0) throw EmptyError();
        return c.pop_front();
    }

    T& front() { return c.front(); }
    const T& front() const { return c.front(); }
    T& back() { return c.back(); }
    const T& back() const { return c.back(); }

    Queue<T>& operator=(const Queue<T>& right) {
        if(&right == this) return *this;
        this->max_length = right.max_length;
        this->c = right.c;
        return *this;
    }
    Queue<T>& operator=(Queue<T>&& right) noexcept {
        if(&right == this) return *this;
        this->max_length = right.max_length;
        this->c = std::move(right.c);
        return *this;
    }
};
}
