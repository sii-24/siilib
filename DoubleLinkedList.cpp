#include <iostream>
#include <memory>

#include "Exception.hpp"


namespace siilib {
template <typename T>
class DoubleLinkedList { 

    struct Object {
        T data;
        Object* next{nullptr};
        Object* prev{nullptr};

        Object(const T& data) : data(data) { }
        Object(T&& data) : data(std::move(data)) { }
    };

    Object* head{nullptr};
    Object* tail{nullptr};
    size_t length{0};

    Object* _at(int index) const {
        if(index < 0) index = static_cast<int>(length) + index;
        if(index < 0 || index >= static_cast<int>(length)) throw IndexError();
        Object* ptr;
        if(index < length / 2) {
            ptr = head;
            for(int i = 0; i < index; ++i) ptr = ptr->next;
        }
        else {
            ptr = tail;
            for(int i = length - 1; i > index; --i) ptr = ptr->prev;
        }
        return ptr;
    }

    template <typename U>
    T& _push_back(U&& x) {
        try {
            Object* ptr = new Object(std::forward<U>(x));
            if(!tail) {
                head = tail = ptr;
            }
            else {
                tail->next = ptr;
                ptr->prev = tail;
                tail = ptr;
            }
            length++;
        }
        catch(std::bad_alloc&) { throw AllocError(); }
        return tail->data;
    }

    template <typename U>
    T& _push_front(U&& x) {
        try {
            Object* ptr = new Object(std::forward<U>(x));
            if(!head) {
                head = tail = ptr;
            }
            else {
                head->prev = ptr;
                ptr->next = head;
                head = ptr;
            }
            length++;
        }
        catch(std::bad_alloc&) { throw AllocError(); }
        return head->data;
    }

    template <typename U>
    T& _insert(int index, U&& x) {
        if(index == 0) return push_front(std::forward<U>(x));
        if(index == length) return push_back(std::forward<U>(x));
        try {
            Object* ptr = new Object(std::forward<U>(x));
            Object* left = _at(index - 1);
            Object* right = left->next;
            left->next = ptr;
            right->prev = ptr;
            ptr->prev = left;
            ptr->next = right;
            length++;
            return ptr->data;
        }
        catch(std::bad_alloc&) { throw AllocError(); }
    }


public:
    DoubleLinkedList() { }
    DoubleLinkedList(const T* ar, size_t len) {
        for(size_t i = 0; i < len; ++i) this->push_back(ar[i]);
    }
    DoubleLinkedList(const DoubleLinkedList<T>& right) {
        for(Object* ptr = right.head; ptr != nullptr; ptr = ptr->next) this->push_back(ptr->data);
    }
    DoubleLinkedList(DoubleLinkedList<T>&& right) noexcept {
        this->head = right.head;
        this->tail = right.tail;
        this->length = right.length;
        right.head = right.tail = nullptr;
        right.length = 0;
    }
    DoubleLinkedList(std::initializer_list<T> ar) {
        for(const T& x : ar) this->push_back(x);
    }

    ~DoubleLinkedList() {
        this->clear();
    }


    void clear() {
        Object* ptr = head;
        while(head) {
            ptr = head;
            head = head->next;
            delete ptr;
        }
        head = tail = nullptr;
        length = 0;
    }

    bool is_empty() const { return length == 0; }

    size_t get_length() const { return length; }


    T& push_back(const T& x) {
        return _push_back(x);
    }
    T& push_back(T&& x) {
        return _push_back(std::move(x));
    }

    T& push_front(const T& x) {
        return _push_front(x);
    }
    T& push_front(T&& x) {
        return _push_front(std::move(x));
    }
    T pop_back() {
        if(!tail) throw EmptyError();
        T res = std::move(tail->data);
        if(head == tail) {
            delete tail;
            head = tail = nullptr;
        }
        else {
            Object* ptr = tail->prev;
            ptr->next = nullptr;
            delete tail;
            tail = ptr;
        }
        length--;
        return res;
    }
    T pop_front() {
        if(!head) throw EmptyError();
        T res = std::move(head->data);
        if(head == tail) {
            delete head;
            head = tail = nullptr;
        }
        else {
            Object* ptr = head->next;
            ptr->prev = nullptr;
            delete head;
            head = ptr;
        }
        length--;
        return res;
    }

    T& insert(int index, const T& x) {
        return _insert(index, x);
    }
    T& insert(int index, T&& x) {
        return _insert(index, std::move(x));
    }
    T erase(int index) {
        if(index == 0) return pop_front();
        if(index == length-1) return pop_back();
        Object* ptr = _at(index);
        T res = std::move(ptr->data);
        ptr->prev->next = ptr->next;
        ptr->next->prev = ptr->prev;
        delete ptr;
        length--;
        return res;
    }

    bool remove(const T& key) {
        Object* ptr = head;
        for(size_t i = 0; i < length; ++i) {
            if(ptr->data == key) {
                this->erase(i);
                return true;
            }
            ptr = ptr->next;
        }
        return false;
    }

    DoubleLinkedList<T>& extend(const DoubleLinkedList<T>& right) {
        for(Object* ptr = right.head; ptr != nullptr; ptr = ptr->next) this->push_back(ptr->data);
        return *this;
    }
    DoubleLinkedList<T>& extend(DoubleLinkedList<T>&& right) {
        if(&right == this) return *this;
        if (!right.head) return *this;
        if (!head) {
            head = right.head;
        }
        else {
            tail->next = right.head;
            right.head->prev = tail;
        }
        tail = right.tail;
        length += right.length;
        right.head = right.tail = nullptr;
        right.length = 0;
        return *this;
    }

    T& operator[](int index) {
        return _at(index)->data;
    }
    const T& operator[](int index) const {
        return _at(index)->data;
    }

    DoubleLinkedList<T>& operator=(const DoubleLinkedList<T>& right) {
        if(&right == this) return *this;
        this->clear();
        for(Object* ptr = right.head; ptr != nullptr; ptr = ptr->next) this->push_back(ptr->data);
        return *this;
    }
    DoubleLinkedList<T>& operator=(DoubleLinkedList<T>&& right) noexcept {
        if(&right == this) return *this;
        this->clear();
        this->length = right.length;
        this->head = right.head;
        this->tail = right.tail;
        right.head = right.tail = nullptr;
        right.length = 0;
        return *this;
    }
    DoubleLinkedList<T>& operator=(std::initializer_list<T> ar) {
        this->clear();
        for(const T& x : ar) this->push_back(x);
        return *this;
    }
};
}
