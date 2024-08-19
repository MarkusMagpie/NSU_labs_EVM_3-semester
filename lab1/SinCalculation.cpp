#include <iostream>
#include <time.h>
#include <cstdlib> // for atoi and atof
#include <cmath> // for pow

long double DegToRad(long double deg) {
    return deg * M_PI / 180;
}

long double SinCalculation(long double x, long long n) {
    long double sin = 0;
    long double prev = x;
    long double sign = 1.0;

    for (long long i = 1; i < n; i++) {
        // term = sign * powl(x, 2 * i + 1);
        // for (long long j = 1; j <= 2 * i + 1; j++) {
        //     term /= j;
        // }
        // sin += term;
        sin += sign * prev;
        prev *= (x * x) / ((2 * i) * (2 * i + 1));
        sign = -sign;
    }

    return sin;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: <angle in degrees> <number of terms>" << std::endl;
        return 0;
    }

    struct timespec start, end;
    long double x = atoll(argv[1]);
    long long n = atoll(argv[2]);

    std::cout << "x = " << x << ", n = " << n << std::endl;

    int runs = 5;
    double time_total = 0;

// 9 223 372 036 854 775 800
    for (long long i = 0; i < runs; i++) {
        clock_gettime(CLOCK_MONOTONIC_RAW, &start);
        double rad_x = DegToRad(x);
        long double sin = SinCalculation(rad_x, n);
        clock_gettime(CLOCK_MONOTONIC_RAW, &end);

        double taken_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
        std::cout << "sin(" << x << ") = " << sin << std::endl;
        std::cout << "Run №" << i + 1 << " took " << taken_time << " seconds to complete" << "\n" << std::endl;
        time_total += taken_time;
    }

    std::cout << "Average time: " << time_total / runs << " seconds" << std::endl;
    return 0;
}