#include <iostream>
#include "vector.hpp"

int main() {
    low::vector<int> v;
    v.emplace_back(7);
    v.emplace_back(8);
//    v.emplace_back(9);

    std::cout << "\n0: " << v[0] << std::endl;
    std::cout << "\n1: " << v[1] << std::endl;
//    std::cout << "\n2: " << v[2] << std::endl;
    return 0;
}
