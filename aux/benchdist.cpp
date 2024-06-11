#include <iostream>
#include <wasserstein.h>
#include <chrono>

const size_t N = 200000;

int main(int argc, char** argv){
    std::vector<double> av = {3.4, 3.9, 7.5, 7.8};
    std::vector<double> aw = {1.4, 0.9, 3.1, 7.2};
    std::vector<double> bv = {4.5, 1.4};
    std::vector<double> bw = {3.2, 3.5};

    double dist;
    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < N; i++) {
        dist = wasserstein(av,aw,bv,bw);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Time: " << elapsed.count() << " seconds" << std::endl;
    std::cout << "The earth movers distance is: " << dist << std::endl;

    start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < N; i++) {
        dist = wasserstein(av,bv);
    }
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;

    std::cout << "Time: " << elapsed.count() << " seconds" << std::endl;
    std::cout << "The earth movers distance is: " << dist << std::endl;


}
