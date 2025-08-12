#include <iostream>
#include <memory>

#include "Exception.cpp"


#define SKIP_STEP 4


namespace siilib {
template <typename T>
class DoubleLinkedList {
public:  

    struct Object {
        T data;
        Object* next{nullptr};
        Object* prev{nullptr};

        Object(const T& data) : data(data) { }
        Object(T&& data) : data(std::move(data)) { }

        T& get_data() { return data; }
        Object* get_next() { return next; }
        Object* get_prev() { return prev; }
    };

private:
    Object* head{nullptr};
    Object* tail{nullptr};
    size_t length{0};

public:
    DoubleLinkedList() { }
    DoubleLinkedList(T* ar, size_t len) {
        for(int i = 0; i < len; i++) this->push_back(ar[i]);
    }
    DoubleLinkedList(const DoubleLinkedList<T>& right) {
        for(Object* ptr = right.head; ptr != nullptr; ptr = ptr->next) this->push_back(ptr->data);
    }
    DoubleLinkedList(DoubleLinkedList<T>&& right) {
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
        Object* tmp;
        while(ptr != nullptr) {
            tmp = ptr;
            ptr = ptr->next;
            delete tmp;
        }
        head = tail = nullptr;
        length = 0;
    }

    bool is_empty() const { return length == 0; }
    Object* get_head() const { 
        if(length == 0) throw EmptyError();
        return head; 
    } 
    Object* get_tail() const { 
        if(length == 0) throw EmptyError();
        return tail; 
    }
    size_t get_length() const { return length; }


    Object& push_back(const T& x) {
        Object* ptr = new Object(x);
        if(!tail) {
            head = tail = ptr;
        }
        else {
            tail->next = ptr;
            ptr->prev = tail;
            tail = ptr;
        }
        length++;
        return *tail;
    }
    Object& push_back(T&& x) {
        Object* ptr = new Object(std::move(x));
        if(!tail) {
            head = tail = ptr;
        }
        else {
            tail->next = ptr;
            ptr->prev = tail;
            tail = ptr;
        }
        length++;
        return *tail;
    }

    Object& push_front(const T& x) {
        Object* ptr = new Object(x);
        if(!head) {
            head = tail = ptr;
        }
        else {
            head->prev = ptr;
            ptr->next = head;
            head = ptr;
        }
        length++;
        return *head;
    }
    Object& push_front(T&& x) {
        Object* ptr = new Object(std::move(x));
        if(!head) {
            head = tail = ptr;
        }
        else {
            head->prev = ptr;
            ptr->next = head;
            head = ptr;
        }
        length++;
        return *head;
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
            delete tail;
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

    Object& insert(int index, const T& x) {
        if(index < 0) index = static_cast<int>(length) + index;
        if(index < 0 || index > static_cast<int>(length)) throw IndexError();
        if(index == 0) return push_front(x);
        if(index == length) return push_back(x);
        Object* left;
        if(index < length / 2) {
            left = head;
            for(int i = 0; i < index-1; i++) left = left->next;
        }
        else {
            left = tail;
            for(int i = length - 1; i > index-1; i--) left = left->prev;
        }
        Object* right = left->next;
        Object* ptr = new Object(x);
        left->next = ptr;
        right->prev = ptr;
        ptr->prev = left;
        ptr->next = right;
        length++;
        return *ptr;
    }
    Object& insert(int index, T&& x) {
        if(index < 0) index = static_cast<int>(length) + index;
        if(index < 0 || index > static_cast<int>(length)) throw IndexError();
        if(index == 0) return push_front(std::move(x));
        if(index == length) return push_back(std::move(x));
        Object* left;
        if(index < length / 2) {
            left = head;
            for(int i = 0; i < index-1; i++) left = left->next;
        }
        else {
            left = tail;
            for(int i = length - 1; i > index-1; i--) left = left->prev;
        }
        Object* right = left->next;
        Object* ptr = new Object(std::move(x));
        left->next = ptr;
        right->prev = ptr;
        ptr->prev = left;
        ptr->next = right;
        length++;
        return *ptr;
    }
    T erase(int index) {
        if(index < 0) index = static_cast<int>(length) + index;
        if(index < 0 || index >= static_cast<int>(length)) throw IndexError();
        if(index == 0) return pop_front();
        if(index == length-1) return pop_back();
        Object* ptr;
        if(index < length / 2) {
            ptr = head;
            for(int i = 0; i < index; i++) ptr = ptr->next;
        }
        else {
            ptr = tail;
            for(int i = length - 1; i > index; i--) ptr = ptr->prev;
        }
        T res = std::move(ptr->data);
        ptr->prev->next = ptr->next;
        ptr->next->prev = ptr->prev;
        delete ptr;
        length--;
        return res;
    }

    T& operator[](int index) {
        if(index < 0) index = static_cast<int>(length) + index;
        if(index < 0 || index >= static_cast<int>(length)) throw IndexError();
        Object* ptr;
        if(index < length / 2) {
            ptr = head;
            for(int i = 0; i < index; i++) ptr = ptr->next;
        }
        else {
            ptr = tail;
            for(int i = length - 1; i > index; i--) ptr = ptr->prev;
        }
        return ptr->data;
    }
    const T& operator[](int index) const {
        if(index < 0) index = static_cast<int>(length) + index;
        if(index < 0 || index >= static_cast<int>(length)) throw IndexError();
        Object* ptr;
        if(index < length / 2) {
            ptr = head;
            for(int i = 0; i < index; i++) ptr = ptr->next;
        }
        else {
            ptr = tail;
            for(int i = length - 1; i > index; i--) ptr = ptr->prev;
        }
        return ptr->data;
    }

    DoubleLinkedList<T>& operator=(const DoubleLinkedList<T>& right) {
        if(&right == this) return *this;
        this->clear();
        for(Object* ptr = right.head; ptr != nullptr; ptr = ptr->next) this->push_back(ptr->data);
        return *this;
    }
    DoubleLinkedList<T>& operator=(DoubleLinkedList<T>&& right) {
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
    DoubleLinkedList<T>& operator+=(const DoubleLinkedList<T>& right) {
        for(Object* ptr = right.head; ptr != nullptr; ptr = ptr->next) this->push_back(ptr->data);
        return *this;
    }
    DoubleLinkedList<T>& operator+=(DoubleLinkedList<T>&& right) {
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
};
}


int main() {
    using namespace siilib;
    DoubleLinkedList<double> lst; // пустой двусвязный список для хранения данных типа double (структура)

    lst.push_back(double {1.0}); // добавление в конец списка
    lst.push_back(double {3.4}); // добавление в конец списка
    lst.push_front(double {-1.5}); // добавление в начало списка
    lst.insert(1, 78.9);
    lst.erase(2);
    lst.pop_back(); // удаление последнего элемента 
    lst.pop_front(); // удаление первого элемента 


    double d = lst[0];  // получение первого элемента по индексу
    lst[0] = double {5.9}; // запись в первый элемент по индексу

    DoubleLinkedList<int> lst_int; // еще один  двусвязный список для хранения данных типа int

    lst_int.push_back(1); // добавление в конец списка
    lst_int.push_back(2);
    lst_int.push_back(3);

    int var = lst_int[1]; // чтение данных из второго элемента списка
    lst_int[2] = -5; // запись данных в третий элемент списка

    // перебор первого списка
    DoubleLinkedList<double>::Object* ptr_lst = lst.get_head();
    while(ptr_lst) {
        double res = ptr_lst->get_data();
        ptr_lst = ptr_lst->get_next();
    }

    // перебор второго списка
    DoubleLinkedList<int>::Object* ptr_lst_int = lst_int.get_head();
    while(ptr_lst_int) {
        int a = ptr_lst_int->get_data();
        ptr_lst_int = ptr_lst_int->get_next();
    }


    try {
        double cmp = lst[-1];
    }
    catch(const IndexError& e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
