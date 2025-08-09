#include <iostream>
#include <memory>

#include "Exception.cpp"


#define SKIP_STEP 4


namespace siilib {
template <typename T>
class OneLinkedList {
public:   
    class Object {
    public:
        T data;
        std::shared_ptr<Object> next{nullptr};

        Object(const T& data, std::shared_ptr<Object> next=nullptr) : data(data), next(next) { }

        T& get_data() { return data; } // получение значения поля data
        std::shared_ptr<Object> get_next() { return next; } // получение значения поля next
    };

private:
    std::shared_ptr<Object> head{nullptr};
    std::shared_ptr<Object> tail{nullptr};
    size_t length{0};

public:
    OneLinkedList() { }
    OneLinkedList(T* ar, size_t len) {
        for(int i = 0; i < len; i++) this->push_back(ar[i]);
    }
    OneLinkedList(const OneLinkedList<T>& right) {
        for(std::shared_ptr<Object> ptr = right.head; ptr != nullptr; ptr = ptr->next) this->push_back(ptr->data);
    }
    OneLinkedList(OneLinkedList<T>&& right) {
        this->head = right.head;
        this->tail = right.tail;
        this->length = right.length;
        right.head = right.tail = nullptr;
        right.length = 0;
    }
    OneLinkedList(std::initializer_list<T> ar) {
        for(const T& x : ar) this->push_back(x);
    }

    void clear() { length = 0; head = tail = nullptr; }

    bool is_empty() const { return length == 0; }
    std::shared_ptr<Object>  get_head() const { 
        if(length == 0) throw EmptyError();
        return head; 
    } 
    std::shared_ptr<Object>  get_tail() const { 
        if(length == 0) throw EmptyError();
        return tail; 
    }
    size_t get_length() const { return length; }

    Object& push_back(const T& x) {
        if(!head) {
            tail = head = std::make_shared<Object>(x);
        }
        else {
            tail->next = std::make_shared<Object>(x);
            tail = tail->next;
        }
        length++;
        return *tail;
    }
    Object& push_front(const T& x) {
        if(!head) {
            tail = head = std::make_shared<Object>(x);
        }
        else {
            head = std::make_shared<Object>(x, head);
        }
        length++;
        return *head;
    }
    Object& push_back(T&& x) {
        if(!head) {
            tail = head = std::make_shared<Object>(std::move(x));
        }
        else {
            tail->next = std::make_shared<Object>(std::move(x));
            tail = tail->next;
        }
        length++;
        return *tail;
    }
    Object& push_front(T&& x) {
        if(!head) {
            tail = head = std::make_shared<Object>(std::move(x));
        }
        else {
            head = std::make_shared<Object>(std::move(x), head);
        }
        length++;
        return *head;
    }
    T pop_back() {
        if(!head) throw EmptyError();
        std::shared_ptr<Object> ptr = tail;
        length--;
        if(head == tail) {
            head = tail = nullptr;
            return ptr->data;
        }
        tail = head;
        while(tail->next != ptr) tail = tail->next;
        tail->next = nullptr;
        return std::move(ptr->data);
    }
    T pop_front() {
        if(!head) throw EmptyError();
        std::shared_ptr<Object> ptr = head;
        length--;
        if(head == tail) {
            head = tail = nullptr;
            return ptr->data;
        }
        head = head->next;
        return std::move(ptr->data);
    }

    Object& insert(int index, const T& x) {
        if(index < 0) index = static_cast<int>(length) + index;
        if(index < 0 || index > static_cast<int>(length)) throw IndexError();
        if(index == 0) {
            head = std::make_shared<Object>(x, head);
            length++;
            return *head;
        }
        if(index == length) {
            tail->next = std::make_shared<Object>(x);
            tail = tail->next;
            length++;
            return *tail;
        }
        Object* ptr = head.get();
        for(int i = 0; i < index-1; i++) ptr = ptr->next.get();
        ptr->next = std::make_shared<Object>(x, ptr->next);
        if(!ptr->next->next) tail = ptr->next;
        length++;
        return *ptr->next;
    }
    Object& insert(int index, T&& x) {
        if(index < 0) index = static_cast<int>(length) + index;
        if(index < 0 || index > static_cast<int>(length)) throw IndexError();
        if(index == 0) {
            head = std::make_shared<Object>(std::move(x), head);
            length++;
            return *head;
        }
        if(index == length) {
            tail->next = std::make_shared<Object>(std::move(x));
            tail = tail->next;
            length++;
            return *tail;
        }
        Object* ptr = head.get();
        for(int i = 0; i < index-1; i++) ptr = ptr->next.get();
        ptr->next = std::make_shared<Object>(std::move(x), ptr->next);
        if(!ptr->next->next) tail = ptr->next;
        length++;
        return *ptr->next;
    }
    T remove(int index) {
        if(index < 0) index = static_cast<int>(length) + index;
        if(index < 0 || index >= static_cast<int>(length)) throw IndexError();
        std::shared_ptr<Object> tmp = head;
        length--;
        if(index == 0) {
            head = head->next;
            if (!head) tail = nullptr;
            return tmp->data;
        }
        std::shared_ptr<Object> ptr = head;
        for(int i = 0; i < index-1; i++) ptr = ptr->next;
        tmp = ptr->next;
        if(!ptr->next->next) {
            tail = ptr;
            ptr->next = nullptr;
        }
        else {
            ptr->next = ptr->next->next;
        }
        return std::move(tmp->data);
    }

    T& operator[](int index) {
        if(index < 0) index = static_cast<int>(length) + index;
        if(index < 0 || index >= static_cast<int>(length)) throw IndexError();
        Object* ptr = head.get();
        for(int i = 0; i < index; i++) ptr = ptr->next.get();
        return ptr->data;
    }
    const T& operator[](int index) const {
        if(index < 0) index = static_cast<int>(length) + index;
        if(index < 0 || index >= static_cast<int>(length)) throw IndexError();
        Object* ptr = head.get();
        for(int i = 0; i < index; i++) ptr = ptr->next.get();
        return ptr->data;
    }

    OneLinkedList<T>& operator=(const OneLinkedList<T>& right) {
        if(&right == this) return *this;
        this->length = 0;
        this->head = this->tail = nullptr;
        for(Object* ptr = right.head.get(); ptr != nullptr; ptr = ptr->next) this->push_back(ptr->data);
        return *this;
    }
    OneLinkedList<T>& operator=(OneLinkedList<T>&& right) {
        if(&right == this) return *this;
        this->length = right.length;
        this->head = right.head;
        this->tail = right.tail;
        right.head = right.tail = nullptr;
        right.length = 0;
        return *this;
    }
    OneLinkedList<T>& operator+=(const OneLinkedList<T>& right) {
        for(Object* ptr = right.head.get(); ptr != nullptr; ptr = ptr->next) this->push_back(ptr->data);
        return *this;
    }
    OneLinkedList<T>& operator+=(OneLinkedList<T>&& right) {
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
};
}


int main() {
    using namespace siilib;
    OneLinkedList<double> lst; // пустой односвязный список для хранения данных типа double (структура)

    lst.push_back(double {1.0}); // добавление в конец списка
    lst.push_back(double {3.4}); // добавление в конец списка
    lst.push_front(double {-1.5}); // добавление в начало списка
    lst.pop_back(); // удаление последнего элемента (если его нет, то ничего не делать)
    lst.pop_front(); // удаление первого элемента (если его нет, то ничего не делать)

    double d = lst[0];  // получение первого элемента по индексу
    lst[0] = double {5.9}; // запись в первый элемент по индексу

    OneLinkedList<int> lst_int; // еще один односвязный список для хранения данных типа int

    lst_int.push_back(1); // добавление в конец списка
    lst_int.push_back(2);
    lst_int.push_back(3);

    int var = lst_int[1]; // чтение данных из второго элемента списка
    lst_int[2] = -5; // запись данных в третий элемент списка

    // перебор первого списка
    std::shared_ptr<OneLinkedList<double>::Object> ptr_lst = lst.get_head();
    while(ptr_lst) {
        double res = ptr_lst->get_data();
        ptr_lst = ptr_lst->get_next();
    }

    // перебор второго списка
    std::shared_ptr<OneLinkedList<int>::Object> ptr_lst_int = lst_int.get_head();
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
