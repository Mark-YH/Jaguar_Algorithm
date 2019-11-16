#include "JA.h"
#include <iostream>
#include <chrono>

int main() {
    std::remove("../log/ja.epin");
    std::remove("../log/hunting1.csv");
    std::remove("../log/see_around.csv");
    std::remove("../log/speed_down.csv");
    std::remove("../log/speed_up.csv");
    auto start = std::chrono::steady_clock::now();
    srand(114);
    JA ja;
    ja.run();
    auto end = std::chrono::steady_clock::now();
    std::cout << "Time taken: " << std::chrono::duration<double>(end - start).count() << "s" << std::endl;
    return 0;
}