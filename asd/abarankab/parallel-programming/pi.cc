#include <iostream>
#include <omp.h>
#include <vector>

#include "benchmark.h"
#include "defs.h"
#include "timer.h"

const u64 num_steps = 100'000'000;
const long double delta = 1.0 / num_steps;

// Bad version
void par_pi_bad() {
    double res = 0;
    escape(&res);

    std::cout << "Threads: " << omp_get_max_threads() << "\n";
    std::vector<long double> local_sum(omp_get_max_threads());

    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int num_threads = omp_get_num_threads();

        size_t L = id * num_steps / num_threads;
        size_t R = (id + 1) * num_steps / num_threads;

        for (size_t step = L; step < R; ++step) {
            local_sum[omp_get_thread_num()] += 4 / (1 + (delta * step) * ((delta * step))) * delta;
        }
    }

    for (size_t i = 0; i < (size_t)omp_get_max_threads(); ++i) {
        std::cout << "Thread no. " << i << " produced " << local_sum[i] << "\n";
        res += local_sum[i];
    }

    escape(&res);
    std::cout << "Parallel " << res << "\n";
}

// Better version
void par_pi_med() {
    double res = 0;
    escape(&res);

    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int num_threads = omp_get_num_threads();
        double local_res = 0;

        size_t L = id * num_steps / num_threads;
        size_t R = (id + 1) * num_steps / num_threads;

        for (size_t step = L; step < R; ++step) {
            local_res += 4 / (1 + (delta * step) * ((delta * step))) * delta;
        }

        #pragma omp atomic
        res += local_res;
    }

    escape(&res);
    std::cout << "Parallel " << res << "\n";
}

// Incorrect version
void par_pi_mistake() {
    double res = 0;
    escape(&res);

    #pragma omp parallel for
    for (size_t step = 0; step < num_steps; ++step) {
        res += 4 / (1 + (delta * step) * ((delta * step))) * delta;  // Multiple threads write to the same location
    }

    escape(&res);
    std::cout << "Parallel " << res << "\n";
}

void par_pi_good() {
    double res = 0;
    escape(&res);

    #pragma omp parallel for reduction(+:res)
    for (size_t step = 0; step < num_steps; ++step) {
        res += 4 / (1 + (delta * step) * ((delta * step))) * delta;
    }

    escape(&res);
    std::cout << "Parallel " << res << "\n";
}

void seq_pi() {
    long double res = 0;
    escape(&res);

    for (size_t step = 0; step < num_steps; ++step) {
        res += 4 / (1 + (delta * step) * ((delta * step))) * delta;
    }

    escape(&res);
    std::cout << "Sequential " << res << "\n";
}

int main() {
    u64 seq_time, par_time;

    {
        u64 start_time = currentSeconds();
        seq_pi();
        u64 end_time = currentSeconds();
        seq_time = end_time - start_time;
    }

    std::cout << "\n-----\n\n";

    {
        u64 start_time = currentSeconds();
        par_pi_good();
        u64 end_time = currentSeconds();
        par_time = end_time - start_time;
    }

    std::cout << "\n-----\n\n";

    std::cout << "Sequential: " << seq_time << " ns\n"
              << "Parallel: " << par_time << " ns\n"
              << "Parallel is " << (double)seq_time / par_time << " times faster\n";
    
    return 0;
}