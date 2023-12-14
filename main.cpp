#include <QCoreApplication>
#include <iostream>
#include <vector>
#include <thread>
#include <numeric>
#include <chrono>
#define ARRAY_SIZE 10000000


void calculateSumMultithreading(std::vector<int>& arr) {
    auto start = std::chrono::high_resolution_clock::now();
    int numThreads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads(numThreads);

    int chunkSize = arr.size() / numThreads;
    int sum = 0;

    for (int i = 0; i < numThreads; ++i) {
        threads[i] = std::thread([&, i]() {
            int start = i * chunkSize;
            int end = (i == numThreads - 1) ? arr.size() : start + chunkSize;
            int localSum = std::accumulate(arr.begin() + start, arr.begin() + end, 0);

            #pragma omp critical
            sum += localSum;
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    std::cout << "Multithreading sum: " << sum << std::endl;
    auto end = std::chrono::high_resolution_clock::now();


       std::chrono::duration<double> duration = end - start;


       std::cout << "Time: " << duration.count() << " sec" << std::endl;

}


void calculateSumOpenMP(std::vector<int>& arr) {
     auto start = std::chrono::high_resolution_clock::now();
    int sum = 0;

    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < arr.size(); ++i) {
        sum += arr[i];
    }

    std::cout << "OpenMP sum: " << sum << std::endl;
    auto end = std::chrono::high_resolution_clock::now();


       std::chrono::duration<double> duration = end - start;


       std::cout << "Time: " << duration.count() << " sec" << std::endl;


}



int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    std::vector<int> arr(ARRAY_SIZE, 1);

        calculateSumMultithreading(arr);
        calculateSumOpenMP(arr);
    return a.exec();
}
