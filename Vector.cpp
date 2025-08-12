#include <iostream>
#include <memory>

#include "Exception.cpp"


#define MIN_CAPACITY 8


namespace siilib {
template<typename T>
class Vector {
    std::shared_ptr<T[]> data{nullptr};
    size_t length{0};
    size_t capacity{0};
    unsigned resize_factor{2};
    bool manual_memory{false};

public:
    Vector(size_t capacity=MIN_CAPACITY, unsigned resize_factor=2) : length(0), capacity(capacity), resize_factor(resize_factor < 2 ? 2 : resize_factor), manual_memory(capacity != MIN_CAPACITY) {
        data = std::make_shared<T[]>(capacity);
    }
    Vector(T ar[], size_t len, unsigned resize_factor=2) : length(len), capacity(MIN_CAPACITY), resize_factor(resize_factor < 2 ? 2 : resize_factor), manual_memory(false) {
        while(capacity <= length) capacity *= resize_factor;
        data = std::make_shared<T[]>(capacity);
        for(size_t i = 0; i < length; i++) {
            data[i] = ar[i];
        }
    }
    Vector(const Vector<T>& right) {
        this->length = right.length;
        this->capacity = right.capacity;
        this->resize_factor = right.resize_factor;
        this->manual_memory = right.manual_memory;
        this->data = std::make_shared<T[]>(capacity);
        for(size_t i = 0; i < length; i++) {
            this->data[i] = right.data[i];
        }
    }
    Vector(Vector<T>&& right) {
        this->length = right.length;
        this->capacity = right.capacity;
        this->resize_factor = right.resize_factor;
        this->manual_memory = right.manual_memory;
        this->data = right.data;
        right.length = 0;
        right.capacity = 0;
    }
    Vector(std::initializer_list<T> ar) : length(ar.size()), capacity(MIN_CAPACITY), resize_factor(2), manual_memory(false) {
        while(capacity <= length) capacity *= resize_factor;
        data = std::make_shared<T[]>(capacity);
        size_t i = 0;
        for (const T& val : ar) {
            data[i++] = val;
        }
    }

    void resize(size_t len=0, bool manual_memory=true) {
        std::shared_ptr<T[]> ptr;
        try {
            if(len > length) {
                if(manual_memory) this->manual_memory = true;
                size_t ptr_capacity = capacity;
                if(len > capacity) {
                    while(ptr_capacity <= len) ptr_capacity *= resize_factor;
                }
                if(len < capacity) {
                    while(ptr_capacity >= len) ptr_capacity /= resize_factor;
                    ptr_capacity = ptr_capacity > MIN_CAPACITY ? ptr_capacity : MIN_CAPACITY;
                }
                ptr = std::make_shared<T[]>(ptr_capacity);
                capacity = ptr_capacity;
            }
            else if(length == capacity) {
                ptr = std::make_shared<T[]>(capacity * resize_factor);
                capacity *= resize_factor;
            }
            else if(length < capacity / resize_factor && capacity > MIN_CAPACITY && !this->manual_memory) {
                ptr = std::make_shared<T[]>(capacity / resize_factor);
                capacity /= resize_factor;
            }
            else return;
        }
        catch(const std::bad_alloc&) { throw ResizeError(); }
        for(size_t i = 0; i < length; i++) {
            ptr[i] = data[i];
        }
        data = ptr;
    }
    void set_resize_factor(unsigned resize_factor) { this->resize_factor = resize_factor < 2 ? 2 : resize_factor; }
    void clear() { length = 0; }

    size_t get_capacity() const { return capacity; }
    size_t get_length() const { return length; }
    size_t get_size() const { return capacity * sizeof(T); }
    size_t get_resize_factor() const { return resize_factor; }
    bool is_empty() const { return length == 0; }

    T& push_back(T&& x) {
        if(length == capacity) this->resize();
        return data[length++] = std::move(x);
    }
    T& push_front(T&& x) {
        if(length == capacity) this->resize();
        for(size_t i = length; i > 0; i--) {
            data[i] = data[i-1];
        }
        length++;
        return data[0] = std::move(x);
    }
    T& push_back(const T& x) {
        if(length == capacity) this->resize();
        return data[length++] = x;
    }
    T& push_front(const T& x) {
        if(length == capacity) this->resize();
        for(size_t i = length; i > 0; i--) {
            data[i] = data[i-1];
        }
        length++;
        return data[0] = x;
    }
    T pop_back() {
        if(length == 0) throw EmptyError();
        length--;
        this->resize();
        return data[length];
    }
    T pop_front() {
        if(length == 0) throw EmptyError();
        T ptr = data[0];
        for(size_t i = 0; i < length - 1; i++) {
            data[i] = data[i+1];
        }
        length--;
        this->resize();
        return ptr;
    }

    T& insert(int index, const T& x) {
        if(index < 0) index = static_cast<int>(length) + index;
        if(index < 0 || index > static_cast<int>(length)) throw IndexError();
        this->resize();
        for(size_t i = length; i > index; i--) {
            data[i] = data[i-1];
        }
        length++;
        return data[index] = x;
    }
    T& insert(int index, T&& x) {
        if(index < 0) index = static_cast<int>(length) + index;
        if(index < 0 || index > static_cast<int>(length)) throw IndexError();
        this->resize();
        for(size_t i = length; i > index; i--) {
            data[i] = data[i-1];
        }
        length++;
        return data[index] = std::move(x);
    }
    T remove(int index) {
        if(index < 0) index = static_cast<int>(length) + index;
        if(index < 0 || index >= static_cast<int>(length)) throw IndexError();
        T ptr = data[index];
        for(size_t i = index; i < length - 1; i++) {
            data[i] = data[i+1];
        }
        length--;
        this->resize();
        return ptr;
    }

    T& operator[](int index) {
        if(index < 0) index = static_cast<int>(length) + index;
        if(index < 0 || index >= static_cast<int>(length)) throw IndexError();
        return data[index];
    }
    const T& operator[](int index) const { 
        if(index < 0) index = static_cast<int>(length) + index;
        if(index < 0 || index >= static_cast<int>(length)) throw IndexError();
        return data[index];
    }

    Vector<T>& operator=(const Vector<T>& right) {
        if(&right == this) return *this;
        this->length = right.length;
        this->capacity = right.capacity;
        this->resize_factor = right.resize_factor;
        this->manual_memory = right.manual_memory;
        this->data = std::make_shared<T[]>(capacity);
        for(size_t i = 0; i < length; i++) {
            this->data[i] = right.data[i];
        }
        return *this;
    }
    Vector<T>& operator=(Vector<T>&& right) {
        if(&right == this) return *this;
        this->length = right.length;
        this->capacity = right.capacity;
        this->resize_factor = right.resize_factor;
        this->manual_memory = right.manual_memory;
        this->data = right.data;
        right.length = 0;
        right.capacity = 0;
        return *this;
    }
    Vector<T>& operator=(std::initializer_list<T> ar) {
        //
    }
    Vector<T>& operator+=(const Vector<T>& right) {
        this->resize(this->length + right.length, false);
        for(size_t i = 0; i < right.length; i++) {
            this->data[length++] = right.data[i];
        }
        return *this;
    }

    Vector<T> operator+(const Vector<T>& right) const {
        Vector<T> res{*this};
        res.resize(res.length + right.length, false);
        for(size_t i = 0; i < right.length; i++) {
            res.data[res.length++] = right.data[i];
        }
        return res;
    }
};
}


int main() {
    using namespace siilib;
    Vector<int> v;
    for(int i = 0; i < 20; i++) {
        v.push_back(i);
    }
    for(int i = 0; i < 20; i++) {
        v.pop_back();
    } 

    Vector<int> v2(20);
    for(int i = 0; i < 20; i++) {
        v2.push_back(i);
    }
    for(int i = 0; i < 20; i++) {
        v2.pop_back();
    } 
    v2.resize(9);
    Vector<short> ar_d; // создание пустого динамического массива (length = 0, capacity = MIN_CAPACITY)

    ar_d.push_back(1); // добавление значения в конец
    ar_d.push_back(1); 
    ar_d.push_front(2); // добавление значения в начало 
    ar_d.push_front(2);
    ar_d.pop_back(); // удаление последнего элемента
    ar_d.pop_front(); // удаление первого элемента

    ar_d[0] = 5;       // изменение значения первого элемента
    short d = ar_d[1]; // считывание значения второго элемента

    ar_d[-1] = 78;
    std::cout << ar_d[-1] << std::endl;

    ar_d.insert(-1, 129);
    for(int i = 0; i < ar_d.get_length(); i++) {
        std::cout << ar_d[i] << " ";
    }
    std::cout << std::endl;
    ar_d.insert(1, 70); // вставка элемента по индексу indx со значением 70
    ar_d.remove(1); // удаление элемента по индексу indx

    size_t cap = ar_d.get_capacity(); // возвращает поле capacity
    size_t len = ar_d.get_size(); // возвращает поле length

    Vector<short> ar_d2 = ar_d; // копирование (конструктор копирования)
    ar_d2 += ar_d; // добавление значений массива ar_d в конец массива ar_d2
    Vector<short> res = ar_d2 + ar_d; // соединение двух массивов (сами массивы ar_d, ar_d2 не меняются)
    if( !ar_d2.is_empty() ) // метод empty возвращает true, если массив пуст, и false иначе
    {
        ar_d2.clear(); // удаление всех элементов из массива (параметр length равен 0)
    }

    // перебор всех сохраненных значений в динамическом массиве ar_d
    size_t sz = ar_d.get_length();
    for(size_t i = 0; i < sz; ++i)
        std::cout << ar_d[i] << " ";

    try {
        short v1 = ar_d[-1];
    }
    catch(const IndexError& e) {
        std::cout << e.what() << std::endl;
    }

    try {
        ar_d.insert(-2, 18);
    }
    catch(const IndexError& e) {
        std::cout << e.what() << std::endl;
    }

    try {
        ar_d.remove(100);
    }
    catch(const IndexError& e) {
        std::cout << e.what() << std::endl;
    } 

    return 0;
}
