#include <iostream>
#include <omp.h>
#include <random>
#include <vector>

#include "benchmark.h"
#include "defs.h"
#include "timer.h"

std::random_device rd;
std::mt19937 gen(rd());

std::vector<u64> init_data() {
    static const u64 SIZE = 100'000'000;
    static const u64 MAX_ELEMENT = 1'000'000;
    static std::uniform_int_distribution<u64> distribution(1, MAX_ELEMENT);

    std::vector<u64> data(SIZE);

    #pragma omp parallel for private(gen)
    for (u64& x : data) {
        x = distribution(gen);
    }
    return data;
}

u64 par_sum(std::vector<u64>& v) {
    u64 res = 0;

    // This version runs slower because threads stop each other
    // 
    // u64 local_res = 0; 
    // #pragma omp parallel private(local_res)
    // {
    //     #pragma omp for
    //     for (i = 0; i < v.size(); ++i) {
    //         local_res = local_res + v[i];
    //     }
    //     #pragma omp critical
    //     {
    //         res += local_res;
    //     }
    // }

    #pragma omp parallel for reduction(+:res)
    for (size_t i = 0; i < v.size(); ++i) {
        res += v[i];
    }

    return res;
}

u64 seq_sum(std::vector<u64>& v) {
    u64 res = 0;
    for (auto x : v) res += x;
    return res;
}

int main() {
    const size_t NUM_CYCLES = 10;
    const size_t CYCLE_STOP = NUM_CYCLES / 10;

    u64 par_time = 0;
    u64 seq_time = 0;

    std::cout << "Start\n" << "Threads: " << omp_get_max_threads() << "\n\n";

    for (size_t cycle = 1; cycle <= NUM_CYCLES; ++cycle) {
        std::vector<u64> data = init_data();
        u64 par_result, seq_result;

        {
            escape(&data);
            u64 start_time = currentSeconds();
            par_result = par_sum(data);
            u64 end_time = currentSeconds();
            par_time += (end_time - start_time);
            escape(&par_result);
        }

        {
            escape(&data);
            u64 start_time = currentSeconds();
            seq_result = seq_sum(data);
            u64 end_time = currentSeconds();
            seq_time += (end_time - start_time);
            escape(&seq_result);
        }

        if (par_result != seq_result) {
            std::cerr << std::fixed
                      << "Parallel result is not equal to sequential result:\n"
                      << "Parallel: " << par_result << "\n"
                      << "Sequential: " << seq_result << "\n";
            exit(-1);
        }

        if (cycle % CYCLE_STOP == 0) {
            std::cout << std::fixed
                      << "Step " << cycle / CYCLE_STOP << " of 10:\n"
                      << "Parallel time: " << par_time / cycle << " ns\n"
                      << "Sequential time: " << seq_time / cycle << " ns\n\n";
        }
    }

    std::cout << std::fixed
              << "Final results:\n"
              << "Parallel time: " << par_time / NUM_CYCLES << " ns\n"
              << "Sequential time: " << seq_time / NUM_CYCLES << " ns\n"
              << "Parallelized function is " << (double)seq_time / par_time << " times faster.\n\n";
    return 0;
}
