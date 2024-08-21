#include <iostream>
#include <time.h>
#include <cstdlib> // for atoi and atof
#include <cmath> // for pow

long double ExpCalculation(long double x, long long n) {
    long double exp = 1.0;
    long double term = 1.0;

    for (long long i = 1; i < n; i++) {
        term *= x / i;
        exp += term;
    }

    return exp;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: <degree> <number of terms>" << std::endl;
        return 0;
    }

    struct timespec start, end;
    long double x = atoll(argv[1]);
    long long n = atoll(argv[2]);

    std::cout << "x = " << x << ", n = " << n << std::endl;

    int runs = 3;
    double time_total = 0;

    for (long long i = 0; i < runs; i++) {
        clock_gettime(CLOCK_MONOTONIC_RAW, &start);
        long double exp = ExpCalculation(x, n);
        clock_gettime(CLOCK_MONOTONIC_RAW, &end);

        double taken_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
        std::cout << "e^(" << x << ") = " << exp << std::endl;
        std::cout << "Run №" << i + 1 << " took " << taken_time << " seconds to complete" << "\n" << std::endl;
        time_total += taken_time;
    }

    std::cout << "Average time: " << time_total / runs << " seconds" << std::endl;
    return 0;
}