#include <iostream>

#include "../Array.cpp"


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
