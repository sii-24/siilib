#include "../Vector.cpp"


int main() {
    using namespace siilib;
    Vector<int> v;
    for(int i = 0; i < 20; ++i) {
        v.push_back(i);
    }
    for(int i = 0; i < 20; ++i) {
        v.pop_back();
    } 

    Vector<int> v2(20);
    for(int i = 0; i < 20; ++i) {
        v2.push_back(i);
    }
    for(int i = 0; i < 20; ++i) {
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
    ar_d2.extend(ar_d); // добавление значений массива ar_d в конец массива ar_d2
    //Vector<short> res = ar_d2 + ar_d; // соединение двух массивов (сами массивы ar_d, ar_d2 не меняются)
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
