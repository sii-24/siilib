#include <memory>

#include "Exception.hpp"


namespace siilib {
template <typename T>
class OneLinkedList { 

    struct Object {
        T data;
        Object* next{nullptr};

        Object(const T& data) : data(data) { }
        Object(T&& data) : data(std::move(data)) { }
    };

    Object* head{nullptr};
    Object* tail{nullptr};
    size_t length{0};

    Object* _at(int index) const {
        if(index < 0) index = static_cast<int>(length) + index;
        if(index < 0 || index >= static_cast<int>(length)) throw IndexError();
        Object* ptr = head;
        for(int i = 0; i < index; ++i) ptr = ptr->next;
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
        if(index == 0) return _push_front(std::forward<U>(x));
        if(index == length) return _push_back(std::forward<U>(x));
        try {
            Object* ptr = new Object(std::forward<U>(x));
            Object* left = _at(index - 1);
            Object* right = left->next;
            left->next = ptr;
            ptr->next = right;
            length++;
            return ptr->data;
        }
        catch(std::bad_alloc&) { throw AllocError(); }
    }


public:
    OneLinkedList() { }
    OneLinkedList(const T* ar, size_t len) {
        for(size_t i = 0; i < len; ++i) this->push_back(ar[i]);
    }
    OneLinkedList(const OneLinkedList<T>& right) {
        for(Object* ptr = right.head; ptr != nullptr; ptr = ptr->next) this->push_back(ptr->data);
    }
    OneLinkedList(OneLinkedList<T>&& right) noexcept {
        this->head = right.head;
        this->tail = right.tail;
        this->length = right.length;
        right.head = right.tail = nullptr;
        right.length = 0;
    }
    OneLinkedList(std::initializer_list<T> ar) {
        for(const T& x : ar) this->push_back(x);
    }

    ~OneLinkedList() {
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
            Object* ptr = _at(length-2);
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
        Object* ptr = _at(index-1);
        Object* tmp = ptr->next;
        T res = std::move(tmp->data);
        ptr->next = ptr->next->next;
        delete tmp;
        length--;
        return res;
    }

    void remove(const T& key) {
        Object* prev = nullptr;
        for(Object* ptr = head; ptr != nullptr;) {
            if(ptr->data == key) {
                if(prev) prev->next = ptr->next;
                else head = ptr->next;
                if(ptr == tail) tail = prev;
                delete ptr;
                length--;
                return;
            }
            prev = ptr;
            ptr = ptr->next;
        }
        throw KeyError();
    }

    int find(const T& key) {
        Object* ptr = head;
        for(size_t i = 0; ptr != nullptr; ++i, ptr = ptr->next) {
            if(ptr->data == key) {
                return i;
            }
        }
        throw KeyError();
    }

    OneLinkedList<T>& extend(const OneLinkedList<T>& right) {
        for(Object* ptr = right.head; ptr != nullptr; ptr = ptr->next) this->push_back(ptr->data);
        return *this;
    }
    OneLinkedList<T>& extend(OneLinkedList<T>&& right) {
        if(&right == this) return *this;
        if (!right.head) return *this;
        if (!head) {
            head = right.head;
        }
        else {
            tail->next = right.head;
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

    T& front() {if(!head) throw EmptyError(); return head->data; }
    const T& front() const { if(!head) throw EmptyError();return head->data; }
    T& back() { if(!tail) throw EmptyError();return tail->data; }
    const T& back() const { if(!tail) throw EmptyError();return tail->data; }

    OneLinkedList<T>& operator=(const OneLinkedList<T>& right) {
        if(&right == this) return *this;
        this->clear();
        for(Object* ptr = right.head; ptr != nullptr; ptr = ptr->next) this->push_back(ptr->data);
        return *this;
    }
    OneLinkedList<T>& operator=(OneLinkedList<T>&& right) noexcept {
        if(&right == this) return *this;
        this->clear();
        this->length = right.length;
        this->head = right.head;
        this->tail = right.tail;
        right.head = right.tail = nullptr;
        right.length = 0;
        return *this;
    }
    OneLinkedList<T>& operator=(std::initializer_list<T> ar) {
        this->clear();
        for(const T& x : ar) this->push_back(x);
        return *this;
    }
};
}
