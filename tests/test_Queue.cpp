#include <iostream>
#include <string>

#include "../Queue.cpp"
#include "../Vector.cpp"


int main() {
    using namespace siilib;

    Queue<double> st(10);

    st.push(double {1.0});
    st.push(double {3.4});
    st.push(double {-1.5});
    std::cout << st.front() << std::endl;
    st.pop();
    st.pop();
    std::cout << st.back() << std::endl;

    try {
        for(int i = 0; i < 20; ++i) st.push(i);
    }
    catch(const OverflowError& e) {
        std::cout << e.what() << std::endl;
    }

    Queue<std::string, Vector<std::string>> stv;
    stv.push("abc");
    stv.push("GDZ");
    stv.push("ZZZZZ");
    stv.pop();
    stv.pop();

    return 0;
}
