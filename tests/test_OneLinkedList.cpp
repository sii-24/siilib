#include "../OneLinkedList.cpp"


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

    try {
        double cmp = lst[-1];
    }
    catch(const IndexError& e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
