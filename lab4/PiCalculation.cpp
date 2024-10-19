#include <iostream>
#include <sys/times.h> // for times
#include <unistd.h> // for sysconf
#include <cstdlib> // for atoi and atof

double PiCalculation(long long n) {
    long long m = 0;

    srand(time(NULL));

    for (long long i = 0; i < n; i++) {
        double x = static_cast<double>(rand()) / RAND_MAX * 2 - 1.0;
        double y = static_cast<double>(rand()) / RAND_MAX * 2 - 1.0;
        if (x * x + y * y <= 1.0) {
            m++;
        }
    }

    return 4 * m / n;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Invalid number of arguments" << std::endl;
        return 0;
    }
    
    struct tms start, end;
    long clocks_per_sec = sysconf(_SC_CLK_TCK);
    long long n = atoll(argv[1]);

    std::cout << "N = " << n << std::endl;

    int runs = 5;
    double min_time = 10000;

    for (long long i = 0; i < runs; i++) {
        times(&start);
        double pi = PiCalculation(n);
        times(&end);

        double taken_time = static_cast<double>(end.tms_utime - start.tms_utime) / static_cast<double>(clocks_per_sec);
        std::cout << "pi = " << pi << std::endl;
        std::cout << "Run time: " << taken_time << "\n" << std::endl;
        min_time = std::min(min_time, taken_time);
    }

    std::cout << "Min time: " << min_time << " seconds" << std::endl;
    return 0;
}
