/**
 * This test relies on the same interface that dsu.h gives
 * Class name should be DSU
 * 
 * If you want to check your own implementation of a parallel dsu,
 * feel free to use this one, just modify the include path
 * 
 * If you add no_correctness, no_exceptions or no_performance these
 * tests will be skipped
 * 
 * It checks overall correctness, exceptions and performance
 */

#include <algorithm>
#include <iostream>
#include <omp.h>
#include <pair>
#include <random>
#include <set>
#include <string>
#include <vector>

/* Include your path here */
#include "../benchmark.h"
#include "../defs.h"
#include "../dsu.h"
#include "../timer.h"
#include "../sequential_dsu.h"

std::random_device rd;
std::mt19937 gen(rd());

u32 randint(u32 l, u32 r) {
    return std::uniform_int_distribution<u32>(l, r)(gen);
}

const u32 SMALL_NUM_STEPS = 1'000;
const u32 SMALL_SIZE = 50;

const u32 NUM_STEPS = 1'000;
const u32 MAX_SIZE = 1'000;

const u32 PERF_NUM_STEPS = 20;
const u32 PERF_SIZE = 20'000'000;

void dump_data(const u32 size,
               const std::vector<std::pair<u32, u32>>& queries,
               SequentialDSU correct,
               DSU incorrect,
               u32 a,
               u32 b) {
    std::cerr << "Component mismatch:\n"
                              << "Size: " << size << "\n"
                              << "Queries:\n";
    for (auto p : queries) {
        std::cerr << p.first << " " << p.second << "\n";
    }
    std::cerr << "Correct roots:\n";
    for (u32 i = 0; i < size; ++i) {
        std::cerr << correct.find_root(i) << " ";
    }
    std::cerr << "\nIncorrect roots:\n";
    for (u32 i = 0; i < size; ++i) {
        std::cerr << incorrect.find_root(i) << " ";
    }
    std::cerr << "\nIncorrect parents:\n";
    for (u32 i = 0; i < size; ++i) {
        std::cerr << incorrect.get_parent(i) << " ";
    }
    std::cerr << "\nComponents " << a << " " << b << "\n";
}

void check_correctness() {
    std::cout << std::fixed << "Checking correctness:\n";
    std::cout << "Checking small sizes:\n";
    for (u32 size = 1; size <= SMALL_SIZE; ++size) {
        std::cout << "Size: " << size << "\n";
        for (u32 step = 1; step <= SMALL_NUM_STEPS; ++step) {

            DSU to_check(size);
            SequentialDSU correct(size);

            std::vector<std::pair<u32, u32>> queries(randint(1, size * 1.5));
            for (auto& p : queries) {
                u32 l = randint(0, size - 1);
                u32 r = randint(0, size - 1);
                p.first = std::min(l, r);
                p.second = std::max(l, r);
            }

            for (auto p : queries) {
                correct.unite(p.first, p.second);
            }

            #pragma omp parallel for
            for (auto p : queries) {
                to_check.unite(p.first, p.second);
            }

            for (u32 i = 0; i < size; ++i) {
                for (u32 j = i; j < size; ++j) {
                    if (correct.same_set(i, j) != to_check.same_set(i, j)) {
                        dump_data(size, queries, correct, to_check, i, j);
                        exit(-1);
                    }
                }
            }
        }
    }
    std::cout << "OK\n";

    std::cout << "Checking on random queries:\n";
    for (u32 i = 1; i <= NUM_STEPS; ++i) {
        if (i % (NUM_STEPS / 10) == 1) {
            std::cout << "Step " << i / (NUM_STEPS / 10) + 1 << " of 10\n";
        }

        u32 size = randint(MAX_SIZE / 100, MAX_SIZE);
        DSU to_check(size);
        SequentialDSU correct(size);

        std::vector<std::pair<u32, u32>> queries(randint(1, size * 1.5));  // 1.5 x size operation at max
        for (auto& p : queries) {
            u32 l = randint(0, size - 1);
            u32 r = randint(0, size - 1);
            p.first = std::min(l, r);
            p.second = std::max(l, r);
        }

        for (auto p : queries) {
            correct.unite(p.first, p.second);
        }

        #pragma omp parallel for
        for (auto p : queries) {
            to_check.unite(p.first, p.second);
        }

        for (u32 i = 0; i < size; ++i) {
            for (u32 j = i; j < size; ++j) {
                if (correct.same_set(i, j) != to_check.same_set(i, j)) {
                    dump_data(size, queries, correct, to_check, i, j);
                    exit(-1);
                }
            }
        }
    }
    std::cout << "OK\n";
}

void check_exceptions() {
    std::cout << "Checking exceptions:\n";
    std::cout << "DSU():\n";
    try {
        DSU(0);
    } catch (std::invalid_argument& e) {
        std::cout << "std::invalid_argument\n" << e.what() << "\n";
    } catch (...) {
        std::cerr << "Incorrect exception, expected std::invalid_argument\n";
        exit(-1);
    }
    std::cout << "find_root():\n";
    try {
        DSU d(2);
        d.find_root(2);
    } catch (std::out_of_range& e) {
        std::cout << "std::out_of_range\n" << e.what() << "\n";
    } catch (...) {
        std::cerr << "Incorrect exception, expected std::out_of_range\n";
        exit(-1);
    }

    std::cout << "same_set():\n";
    try {
        DSU d(2);
        d.same_set(0, 2);
    } catch (std::out_of_range& e) {
        std::cout << "std::out_of_range\n" << e.what() << "\n";
    } catch (...) {
        std::cerr << "Incorrect exception, expected std::out_of_range\n";
        exit(-1);
    }

    std::cout << "unite():\n";
    try {
        DSU d(2);
        d.unite(0, 2);
    } catch (std::out_of_range& e) {
        std::cout << "std::out_of_range\n" << e.what() << "\n";
    } catch (...) {
        std::cerr << "Incorrect exception, expected std::out_of_range\n";
        exit(-1);
    }
    std::cout << "OK\n";
}

void check_performance() {
    std::cout << "Checking performance on random queries:\n";
    u64 seq_constructon_time = 0;
    u64 par_constructon_time = 0;
    u64 seq_query_time = 0;
    u64 par_query_time = 0;
    u32 total_queries = 0;
    u64 start, finish;

    for (u32 i = 1; i <= PERF_NUM_STEPS; ++i) {
        if (i % (PERF_NUM_STEPS / 10) == 1) {
            std::cout << "Step " << i / (PERF_NUM_STEPS / 10) + 1 << " of 10\n";
        }

        u32 size = PERF_SIZE;

        /* Sequential construction time */
        escape(&size);
        start = currentSeconds();
        SequentialDSU correct(size);
        finish = currentSeconds();
        escape(&correct);
        seq_constructon_time += finish - start;

        /* Parallel construction time */
        escape(&size);
        start = currentSeconds();
        DSU to_check(size);
        finish = currentSeconds();
        escape(&to_check);
        seq_constructon_time += finish - start;

        std::vector<std::pair<u32, u32>> queries(randint(1, size * 2));  // 1.5 x size operation at max
        total_queries += queries.size();
        for (auto& p : queries) {
            u32 l = randint(0, size - 1);
            u32 r = randint(0, size - 1);
            p.first = std::min(l, r);
            p.second = std::max(l, r);
        }

        /* Sequential queries */
        escape(&queries);
        start = currentSeconds();
        for (auto p : queries) {
            correct.unite(p.first, p.second);
        }
        finish = currentSeconds();
        escape(&correct);
        seq_query_time += finish - start;

        /* Parallel queries */
        escape(&queries);
        start = currentSeconds();
        #pragma omp parallel for
        for (auto p : queries) {
            to_check.unite(p.first, p.second);
        }
        finish = currentSeconds();
        escape(&to_check);
        par_query_time += finish - start;
    }

    std::cout << "Sequential construction time: "
              << static_cast<double>(seq_constructon_time) / PERF_NUM_STEPS << "\n"
              << "Sequential query time: "
              << static_cast<double>(seq_query_time) / PERF_NUM_STEPS << "\n"
              << "Average unite time: "
              << static_cast<double>(seq_query_time) / total_queries << "\n";

    std::cout << "Parallel construction time: "
              << static_cast<double>(par_constructon_time) / PERF_NUM_STEPS << "\n"
              << "Parallel query time: "
              << static_cast<double>(par_query_time) / PERF_NUM_STEPS << "\n"
              << "Average unite time: "
              << static_cast<double>(par_query_time) / total_queries << "\n";
}

int main(int argc, char* argv[]) {
    std::set<std::string> arguments;
    for (int i = 0; i < argc; ++i) arguments.insert(argv[i]);

    bool no_correctness = arguments.count("no_correctness");
    bool no_exceptions = arguments.count("no_exceptions");
    bool no_performance = arguments.count("no_performance");

    if (!no_correctness) {
        check_correctness();
    }

    if (!no_exceptions) {
        check_exceptions();
    }

    if (!no_performance) {
        check_performance();
    }
    return 0;
}