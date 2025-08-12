#include <iostream>
#include <memory>

#include "Exception.cpp"


#define MIN_CAPACITY 8


namespace siilib {
template<typename T>
class Vector {
    T* data{nullptr};
    size_t length{0};
    size_t capacity{0};
    unsigned resize_factor{2};
    bool manual_memory{false};


    void _inc() {
        if(length == capacity) {
            T* ptr;
            try {
                ptr = new T[capacity * resize_factor];
                capacity *= resize_factor;
            }
            catch(const std::bad_alloc&) { throw ResizeError(); }
            for(size_t i = 0; i < length; i++) {
                ptr[i] = std::move(data[i]);
            }
            delete[] data;
            data = ptr;
        }
    }
    void _dec() {
        if(length < capacity / resize_factor && capacity > MIN_CAPACITY && !this->manual_memory) {
            T* ptr;
            try {
                ptr = new T[capacity / resize_factor];
                capacity /= resize_factor;
            }
            catch(const std::bad_alloc&) { throw ResizeError(); }
            for(size_t i = 0; i < length; i++) {
               ptr[i] = std::move(data[i]);
            }
            delete[] data;
            data = ptr;
        }
    }


    template <typename U>
    T& _push_back(U&& x) {
        if(length == capacity) this->_inc();
        return data[length++] = std::forward<U>(x);
    }

    template <typename U>
    T& _push_front(U&& x) {
        if(length == capacity) this->_inc();
        for(size_t i = length; i > 0; i--) {
            data[i] = std::move(data[i-1]);
        }
        length++;
        return data[0] = std::forward<U>(x);
    }

    template <typename U>
    T& _insert(int index, U&& x) {
        if(index < 0) index = static_cast<int>(length) + index;
        if(index < 0 || index > static_cast<int>(length)) throw IndexError();
        if(length == capacity) this->_inc();
        for(size_t i = length; i > index; i--) {
            data[i] = std::move(data[i-1]);
        }
        length++;
        return data[index] = std::forward<U>(x);
    }


public:
    Vector(size_t capacity=MIN_CAPACITY, unsigned resize_factor=2) : length(0), capacity(capacity), resize_factor(resize_factor < 2 ? 2 : resize_factor), manual_memory(capacity != MIN_CAPACITY) {
        data = new T[capacity];
    }
    Vector(T ar[], size_t len, unsigned resize_factor=2) : length(len), capacity(MIN_CAPACITY), resize_factor(resize_factor < 2 ? 2 : resize_factor), manual_memory(false) {
        while(capacity <= length) capacity *= resize_factor;
        data = new T[capacity];
        for(size_t i = 0; i < length; i++) {
            data[i] = ar[i];
        }
    }
    Vector(const Vector<T>& right) {
        this->length = right.length;
        this->capacity = right.capacity;
        this->resize_factor = right.resize_factor;
        this->manual_memory = right.manual_memory;
        this->data = new T[capacity];
        for(size_t i = 0; i < length; i++) {
            this->data[i] = right.data[i];
        }
    }
    Vector(Vector<T>&& right) noexcept {
        this->length = right.length;
        this->capacity = right.capacity;
        this->resize_factor = right.resize_factor;
        this->manual_memory = right.manual_memory;
        this->data = right.data;
        right.length = 0;
        right.capacity = 0;
        right.data = nullptr;
    }
    Vector(std::initializer_list<T> ar) : length(ar.size()), capacity(MIN_CAPACITY), resize_factor(2), manual_memory(false) {
        while(capacity <= length) capacity *= resize_factor;
        data = new T[capacity];
        size_t i = 0;
        for (const T& val : ar) {
            data[i++] = val;
        }
    }
    ~Vector() {
        this->clear();
        delete[] data;
        data = nullptr;
        capacity = 0;
    }

    void clear() { 
        for (size_t i = 0; i < length; ++i) {
            data[i].~T();
        }
        length = 0;
        manual_memory = false;
    }

    void resize(size_t len, bool manual_memory=true) {
        T* ptr;
        if(manual_memory) this->manual_memory = true;
        len = (length > len) ? length : len;
        try {
            size_t tmp_capacity = capacity;
            if(len > capacity) {
                while(tmp_capacity <= len) tmp_capacity *= resize_factor;
            }
            if(len < capacity) {
                while(tmp_capacity >= len) tmp_capacity /= resize_factor;
                tmp_capacity = tmp_capacity > MIN_CAPACITY ? tmp_capacity : MIN_CAPACITY;
            }
            ptr = new T[tmp_capacity];
            capacity = tmp_capacity;
        }
        catch(const std::bad_alloc&) { throw ResizeError(); }
        for(size_t i = 0; i < length; i++) {
            ptr[i] = std::move(data[i]);
        }
        delete[] data;
        data = ptr;
    }

    void set_resize_factor(unsigned resize_factor) { this->resize_factor = resize_factor < 2 ? 2 : resize_factor; }

    size_t get_capacity() const { return capacity; }
    size_t get_length() const { return length; }
    size_t get_size() const { return capacity * sizeof(T); }
    size_t get_resize_factor() const { return resize_factor; }
    bool is_empty() const { return length == 0; }

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
        if(length == 0) throw EmptyError();
        length--;
        if(length < capacity / resize_factor) this->_dec();
        T tmp = std::move(data[length]);
        data[length].~T();
        return tmp;
    }
    T pop_front() {
        if(length == 0) throw EmptyError();
        T tmp = std::move(data[0]);
        for(size_t i = 0; i < length - 1; i++) {
            data[i] = std::move(data[i+1]);
        }
        length--;
        if(length < capacity / resize_factor) this->_dec();
        data[length].~T();
        return tmp;
    }

    T& insert(int index, const T& x) {
        return _insert(index, x);
    }
    T& insert(int index, T&& x) {
        return _insert(index, std::move(x));
    }
    T erase(int index) {
        if(index < 0) index = static_cast<int>(length) + index;
        if(index < 0 || index >= static_cast<int>(length)) throw IndexError();
        T tmp = data[index];
        for(size_t i = index; i < length - 1; i++) {
            data[i] = std::move(data[i+1]);
        }
        length--;
        if(length < capacity / resize_factor) this->_dec();
        return tmp;
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
        try {
            T* tmp = new T[right.capacity];
            delete[] data;
            this->data = tmp;
            this->capacity = right.capacity;
        }
        catch(std::bad_alloc&) { throw AllocError(); }
        this->length = right.length;
        this->resize_factor = right.resize_factor;
        this->manual_memory = right.manual_memory;
        for(size_t i = 0; i < length; i++) {
            this->data[i] = right.data[i];
        }
        return *this;
    }
    Vector<T>& operator=(Vector<T>&& right) noexcept {
        if(&right == this) return *this;
        this->length = right.length;
        this->capacity = right.capacity;
        this->resize_factor = right.resize_factor;
        this->manual_memory = right.manual_memory;
        delete[] data;
        this->data = right.data;
        right.length = 0;
        right.capacity = 0;
        right.data = nullptr;
        return *this;
    }
    Vector<T>& operator=(std::initializer_list<T> ar) {
        try {
            size_t tmp_capacity = MIN_CAPACITY;
            while(tmp_capacity <= ar.size()) tmp_capacity *= resize_factor;
            T* tmp = new T[tmp_capacity];
            delete[] data;
            data = tmp;
            capacity = tmp_capacity;
        }
        catch(std::bad_alloc&) { throw AllocError(); }
        this->clear();
        this->length = ar.size();
        size_t i = 0;
        for (const T& val : ar) {
            data[i++] = val;
        }
        return *this;
    }
    Vector<T>& operator+=(const Vector<T>& right) {
        this->resize(this->length + right.length, false);
        for(size_t i = 0; i < right.length; i++) {
            this->data[length++] = right.data[i];
        }
        return *this;
    }
    Vector<T>& operator+=(Vector<T>&& right) {
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
    ar_d.erase(1); // удаление элемента по индексу indx

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
        ar_d.erase(100);
    }
    catch(const IndexError& e) {
        std::cout << e.what() << std::endl;
    } 

    return 0;
}
