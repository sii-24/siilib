#include <iostream>
#include <memory>

#include "Exception.hpp"
#include "DoubleLinkedList.cpp"


namespace siilib {
template <typename T, typename Container = DoubleLinkedList<T>>
class Stack {

    Container c;
    size_t max_length{0};

public:
    Stack(size_t max_length=0) : c(), max_length(max_length) { }
 
    Stack(const Stack<T>& right) { 
        this->max_length = right.max_length;
        this->c = right.c;
    }
    Stack(Stack<T>&& right) {
        this->max_length = right.max_length;
        this->c = std::move(right.c);
    }

    void clear() { c.clear(); }

    bool is_empty() const { return c.get_length() == 0; }

    size_t get_length() const { return c.get_length(); }


    T& push(const T& x) { 
        if(max_length) if(c.length >= max_length) throw OverflowError();
        return c.push_back(x);
    }
    T& push(T&& x) {
        if(max_length) if(c.get_length() >= max_length) throw OverflowError();
        return c.push_back(std::move(x));
    }

    T pop() { return c.pop_back(); }

    T& top() { return c[-1]; }

    Stack<T>& operator=(const Stack<T>& right) {
        if(&right == this) return *this;
        this->max_length = right.max_length;
        this->c = right.c;
        return *this;
    }
    Stack<T>& operator=(Stack<T>&& right) noexcept {
        if(&right == this) return *this;
        this->max_length = right.max_length;
        this->c = std::move(right.c);
        return *this;
    }
};
}
