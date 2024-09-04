#include <iostream>
#include <time.h>
#include <cstdlib> // for atoi and atof
#include <cmath> // for pow

long double PiCalculation(long long n) {
    long long m = 0;

    srand(time(NULL));
    // srand(static_cast<unsigned int>(time(0))); // устанавливаем значение системных часов в качестве стартового числа

    for (long long i = 0; i < n; i++) {
        // double x = static_cast<double>(rand()) / RAND_MAX // диапозон [0;1]
        // RAND_MAX * 2.0 // увеличиваем диапозон до [0; 2]
        // RAND_MAX * 2.0 - 1.0 // сдвигаем диапозон до [-1; 1] 
        double x = static_cast<double>(rand()) / RAND_MAX * 2.0 - 1.0;
        double y = static_cast<double>(rand()) / RAND_MAX * 2.0 - 1.0;
        if (x * x + y * y <= 1.0) {
            m++;
        }
    }

    return 4.0 * m / n;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Invalid number of arguments" << std::endl;
        return 0;
    }

    struct timespec start, end;
    long long n = atoll(argv[1]);

    std::cout << "N = " << n << std::endl;

    int runs = 5;
    double time_total = 0;

    for (long long i = 0; i < runs; i++) {
        clock_gettime(CLOCK_MONOTONIC_RAW, &start);
        double pi = PiCalculation(n);
        clock_gettime(CLOCK_MONOTONIC_RAW, &end);

        double taken_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
        std::cout << "pi = " << pi << std::endl;
        // std::cout << "Run #" << i + 1 << " took " << taken_time << " seconds to complete" << std::endl;
        std::cout << "Run time: " << taken_time << "\n" << std::endl;
        time_total += taken_time;
    }

    std::cout << "Average time: " << time_total / runs << " seconds" << std::endl;
    return 0;
}
