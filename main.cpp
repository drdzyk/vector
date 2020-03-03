#include <iostream>
#include "vector.hpp"
#include <vector>

int main() {
    low::vector<int> v;
    v.resize(12);

//    for (const auto &e : v)
//    {
//        std::cout << "\ne: " << e << std::endl;
//    }

    std::cout << "\ns: " << v.size() << "; c: " << v.capacity() << std::endl;
    v.resize(9);
    std::cout << "\ns: " << v.size() << "; c: " << v.capacity() << std::endl;
    v.resize(13 );
    std::cout << "\ns: " << v.size() << "; c: " << v.capacity() << std::endl;
//    v.resize(7);
//    std::cout << "\ns: " << v.size() << "; c: " << v.capacity() << std::endl;

    return 0;
}
