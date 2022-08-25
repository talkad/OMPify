#ifndef __DSU_RANKLESS_H
#define __DSU_RANKLESS_H

#include <atomic>
#include <omp.h>
#include <stdexcept>

#include "defs.h"
#include "parallel_array.h"

/**
 * INTERFACE:
 * 
 * DSU(uint32_t N, uint32_t NUM_THREADS) - constructs a DSU of size N using NUM_THREADS
 * uint32_t find_root(uint32_t id) - finds root node of id
 * bool same_set(uint32_t id1, uint32_t id2) - checks if id1 and id2 are in the same set
 * void unite(uint32_t id1, uint32_t id2) - unites sets of id1 and id2
 * 
 * DETAILS:
 * 
 * This implementation relies on path compression only
 * and thus should achieve O(log N) time per query
 */
struct DSU {
    const u32 NUM_THREADS;

    ParallelArray<atomic_u32> parent;

    DSU(u32 size, u32 NUM_THREADS = omp_get_max_threads()) : NUM_THREADS(NUM_THREADS), parent(size) {
        if (size == 0) {
            throw std::invalid_argument("DSU size cannot be zero");
        }

        #pragma omp parallel for shared(parent) num_threads(NUM_THREADS)
        for (u32 i = 0; i < size; ++i) parent[i] = i;
    }

    u32 size() const {
        return parent.size();
    }

    void check_out_of_range(u32 id) const {
        if (id >= size()) {
            throw std::out_of_range("Node id out of range");
        }
    }

    u32 get_parent(u32 id) const {
        return parent[id];
    }

    /**
     * On each step we try to apply path heuristic using CAS
     * and then move closer to the root and
     * 
     * The loop breaks when a node's parent is equal to itself
     * E.g. when we find the root
     */
    u32 find_root(u32 id) {
        check_out_of_range(id);

        while (id != parent[id]) {
            u32 value = parent[id];
            u32 grandparent = parent[value];

            /* Path heuristic */
            if (value != grandparent) {
                parent[id].compare_exchange_strong(value, grandparent);
            }

            id = grandparent;
        }

        return id;
    }

    /**
     * We try to check if two nodes are in the same set
     * by checking if their roots are the same
     * 
     * Since it is a parallel structure, node roots may change during runtime
     * In order to account for this we do a while loop and repeat if
     * our current node is no longer the root of its set
     * 
     * In general, you should call this after synchronization,
     * It still works during parallel segments, but the results will make no sense
     */
    bool same_set(u32 id1, u32 id2) {
        check_out_of_range(id1);
        check_out_of_range(id2);

        while (true) {
            id1 = find_root(id1);
            id2 = find_root(id2);

            if (id1 == id2) {
                return true;
            } else if (parent[id1] == id1) {
                return false;
            }
        }
    }

    /**
     * Since it is a parallel structure, node roots may change during runtime
     * In order to account for this we do a while loop and repeat if
     * the smaller node was updated e.g. when CAS failed
     */
    void unite(u32 id1, u32 id2) {
        check_out_of_range(id1);
        check_out_of_range(id2);

        while (true) {
            id1 = find_root(id1);
            id2 = find_root(id2);

            /* Nodes are already in the same set */
            if (id1 == id2) return;

            if (id1 > id2) {
                std::swap(id1, id2);
            }

            /* If CAS fails we need to repeat the same step once again */
            if (!parent[id2].compare_exchange_strong(id2, id1)) {
                continue;
            }

            break;
        }
    }
};

#endif
