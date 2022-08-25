#ifndef __PREFIX_SUM_H
#define __PREFIX_SUM_H

#include <new>
#include <omp.h>
#include <stdexcept>
#include <vector>

#include "defs.h"
#include "parallel_array.h"

/**
 * INTERFACE:
 * 
 * PrefixSum(uint32_t size, ParallelArray<u32> arr, uint32_t NUM_THREADS) - constructs an array of prefix sums using NUM_THREADS
 * uint32_t operator[i] - returns ith prefix sum = a[0] + ... + a[i]
 * 
 * TODO: I may later add alignment to ParallelArray and use it here
 *       Also now that I think of it this should be a function, not a struct
 */
struct PrefixSum {
    const u32 NUM_THREADS;

    u32 arr_size;
    u32* prefix_sum;

    PrefixSum(u32 arr_size,
              ParallelArray<u32> arr,
              u32 NUM_THREADS = omp_get_max_threads()) : NUM_THREADS(NUM_THREADS),
                                                         arr_size(arr_size) {

        prefix_sum = static_cast<u32*>(operator new[] (arr_size * sizeof(u32),                    // array size
                                                       static_cast<std::align_val_t>(256)));  // alignment

        std::vector<u32> thread_sum(NUM_THREADS);

        #pragma omp parallel num_threads(NUM_THREADS)
        {
            u32 thread_num = omp_get_thread_num();
            u32 current_sum = 0;

            #pragma omp for schedule(static) nowait
            for (u32 i = 0; i < arr_size; ++i) {
                current_sum += arr[i];
                prefix_sum[i] = current_sum;
            }
            thread_sum[thread_num] = current_sum;
            #pragma omp barrier

            u32 offset = 0;
            for (u32 i = 0; i < thread_num; ++i) {
                offset += thread_sum[i];
            }

            // __builtin_assume_aligned(prefix_sum, 256);
            #pragma omp for simd schedule(static)
            for (u32 i = 0; i < arr_size; ++i) {
                prefix_sum[i] += offset;
            }
        }
    }

    u32 size() {
        return arr_size;
    }

    ~PrefixSum() {
        delete[] prefix_sum;
    }

    u32 operator[](u32 id) {
        if (id >= arr_size) {
            throw std::out_of_range("Prefix sum index out of range");
        }
        return prefix_sum[id];
    }
};

#endif
