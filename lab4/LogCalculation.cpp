#include <iostream>
#include <time.h>
#include <cstdlib> // for atoi and atof

long double LogCalculation(long double x, long long n) {
    long double res = 0;
    long double prev = x;
    long double sign = 1.0;

    for (long long i = 1; i <= n; i++) {
        res += sign * prev / i;
        prev *= x;
        sign = -sign;
    }

    return res;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: <base> <number of terms>" << std::endl;
        return 0;
    }


    struct timespec start, end;
    long double x = atoll(argv[1]);

    if (x <= -1 || x > 1) {
        std::cerr << "x must be in the range (-1, 1]" << std::endl;
    }

    long long n = atoll(argv[2]);

    std::cout << "x = " << x << ", n = " << n << std::endl;

    int runs = 5;
    double time_total = 0;

    for (long long i = 0; i < runs; i++) {
        clock_gettime(CLOCK_MONOTONIC_RAW, &start);
        long double log = LogCalculation(x, n);
        clock_gettime(CLOCK_MONOTONIC_RAW, &end);

        double taken_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
        std::cout << "log(" << x << ") = " << log << std::endl;
        std::cout << "Run #" << i + 1 << " took " << taken_time << " seconds to complete" << "\n" << std::endl;
        time_total += taken_time;
    }

    std::cout << "Average time: " << time_total / runs << " seconds" << std::endl;
    return 0;
}