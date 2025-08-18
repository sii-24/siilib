#include <string>

#include "../Stack.cpp"
#include "../Vector.cpp"


int main() {
    using namespace siilib;

    Stack<double> st(10); // пустой стек для хранения данных типа double

    st.push(double {1.0});
    st.push(double {3.4});
    st.push(double {-1.5});
    st.pop();
    st.pop();

    try {
        for(int i = 0; i < 20; ++i) st.push(i);
    }
    catch(const OverflowError& e) {
        std::cout << e.what() << std::endl;
    }

    Stack<std::string, Vector<std::string>> stv;
    stv.push("abc");
    stv.push("GDZ");
    stv.push("ZZZZZ");
    stv.pop();
    stv.pop();

    return 0;
}
