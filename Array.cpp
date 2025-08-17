#include <iostream>
#include <memory>

#include "Exception.cpp"


namespace siilib {
template<typename T>
class Array {
    T* data{nullptr};
    size_t length{0};


    template <typename U>
    T& _insert(int index, U&& x) {
        if(index < 0) index = static_cast<int>(length) + index;
        if(index < 0 || index >= static_cast<int>(length)) throw IndexError();
        for(size_t i = length-1; i > static_cast<size_t>(index); --i) {
            data[i] = std::move(data[i-1]);
        }
        return data[index] = std::forward<U>(x);
    }


public:
    Array(size_t length) {
        try{ 
            data = new T[length];
            this->length = length;
        }
        catch(std::bad_alloc&) { throw AllocError(); }
    }
    Array(T ar[], size_t len, size_t length=0) {
        try {
            size_t tmp_length = length ? length : len;
            data = new T[tmp_length];
            this->length = tmp_length;
            for(size_t i = 0; i < len && i < tmp_length; ++i) {
                data[i] = ar[i];
            }
        }
        catch(std::bad_alloc&) { throw AllocError(); }
    }
    Array(const Array<T>& right) {
        try {
            this->data = new T[right.length];
            this->length = right.length;
            for(size_t i = 0; i < length; ++i) {
                this->data[i] = right.data[i];
            }
        }
        catch(std::bad_alloc&) { throw AllocError(); }
    }
    Array(Array<T>&& right) noexcept {
        this->length = right.length;
        this->data = right.data;
        right.length = 0;
        right.data = nullptr;
    }
    Array(std::initializer_list<T> ar, size_t length=0) {
        try {
            size_t tmp_length = length ? length : ar.size();
            data = new T[tmp_length];
            this->length = tmp_length;
            size_t i = 0;
            for (const T& val : ar) {
                if(i == tmp_length) break;
                data[i++] = val;
            }
        }
        catch(std::bad_alloc&) { throw AllocError(); }
    }
    ~Array() {
        delete[] data;
        data = nullptr;
        length = 0;
    }


    size_t get_length() const { return length; }
    size_t get_size() const { return length * sizeof(T); }


    T& insert(int index, const T& x) {
        return _insert(index, x);
    }
    T& insert(int index, T&& x) {
        return _insert(index, std::move(x));
    }
    T erase(int index) {
        if(index < 0) index = static_cast<int>(length) + index;
        if(index < 0 || index >= static_cast<int>(length)) throw IndexError();
        T tmp = std::move(data[index]);
        for(size_t i = index; i < length - 1; ++i) {
            data[i] = std::move(data[i+1]);
        }
        data[length - 1] = T();
        return tmp;
    }

    bool remove(const T& key) {
        for(size_t i = 0; i < length; ++i) {
            if(data[i] == key) {
                for(size_t j = i; j < length - 1; ++j) {
                    data[j] = std::move(data[j+1]);
                }
                return true;
            }
        }
        return false;
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

    Array<T>& operator=(const Array<T>& right) {
        if(&right == this) return *this;
        for(size_t i = 0; i < length && i < right.length; ++i) {
            this->data[i] = right.data[i];
        }
        return *this;
    }
    Array<T>& operator=(Array<T>&& right) noexcept {
        if(&right == this) return *this;
        delete[] data;
        this->data = right.data;
        this->length = right.length;
        right.length = 0;
        right.data = nullptr;
        return *this;
    }
    Array<T>& operator=(std::initializer_list<T> ar) {
        size_t i = 0;
        for (const T& val : ar) {
            if(i == length) break;
            data[i++] = val;
            }
        return *this;
    }
};
}


int main() {
    using namespace siilib;

    Array<int> ar1(10);
    int digits[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    Array<int> ar2(digits, 10, 15);
    Array<int> ar3{ar1};
    Array<int> ar4{Array<int>(21)};
    Array<int> ar5 = {1, 2, 3, 4, 5, 6, 7};

    size_t length = ar2.get_length();
    size_t size = ar4.get_size();

    ar2.insert(4, -1154);
    ar5.erase(2);

    int d = ar2[8];
    try {
        int d2 = ar2[54];
    }
    catch(IndexError& e) {
        std::cout << e.what() << std::endl;
    }

    ar1 = ar4;
    ar4 = {5, 4, 3, 2, 1};
    return 0;
}
