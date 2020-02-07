#include "JA.h"
#include <iostream>
#include <chrono>
#include <string>

void analyze() {
    const static float best = 0;
    std::ifstream fin("../log/result.csv");
    int count1 = 0, count2 = 0, count3 = 0;
    int avg1 = 0, avg2 = 0;
    int t1, t2;
    double t3;
    char c;
    while (fin >> t1 >> c >> t2 >> c >> t3) {
        if (t1 > 0) {
            avg1 += t1;
            count1++;
        }
        avg2 += t2;
        count2++;
        if (t3 == best) {
            count3++;
        }
    }
    std::cout << std::fixed << setprecision(2);
    std::cout << avg1 / (double) count1 << endl;
    std::cout << avg2 / (double) count2 << endl;
    std::cout << "Found best count (count1): " << count1 << endl;
    std::cout << "Found best count (count3): " << count3 << endl;
    fin.close();
}

int main() {
    std::remove("../log/result.csv");
    std::remove("../log/ja.epin");
    std::remove("../log/hunting.csv");
    std::remove("../log/see_around.csv");
    std::remove("../log/speed_down.csv");
    std::remove("../log/speed_up.csv");
    auto start = std::chrono::steady_clock::now();
    srand(114);
    int k = 0;
    while (k < 1000) {
        JA ja;
        ja.run();
        k++;
    }
    analyze();
    auto end = std::chrono::steady_clock::now();
    std::cout << "Time taken: " << std::chrono::duration<double>(end - start).count() << "s" << std::endl;
    return 0;
}