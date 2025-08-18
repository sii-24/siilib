#include "../DoubleLinkedList.cpp"


int main() {
    using namespace siilib;

    DoubleLinkedList<int> l;
    for(int i = 0; i < 20; ++i) l.push_back(i);
    int i1 = l[4];
    int i2 = l[11];
    int i3 = l[17];
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


    try {
        double cmp = lst[-1];
    }
    catch(const IndexError& e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
