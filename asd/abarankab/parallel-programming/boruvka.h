#ifndef __BORUVKA_H
#define __BORUVKA_H

#include <algorithm>
#include <limits>
#include <omp.h>
#include <pair>
#include <unordered_map>

#include "dsu.h"
#include "sequential_dsu.h"
#include "graph.h"
#include "parallel_array.h"
#include "prefix_sum.h"

struct BoruvkaMST {
    /**
     * I use the same atomic pair that I use in dsu.h
     * TODO: create a special class for this
     */
    const u32 EDGE_BINARY_BUCKET_SIZE = 32;
    const u64 EDGE_WEIGHT_MASK = 0xFFFFFFFF00000000ULL;

    u64 encode_edge(u32 id, u32 weight) {
        return (static_cast<u64>(weight) << EDGE_BINARY_BUCKET_SIZE) + id;
    }

    u32 get_id(u64 encoded_edge) {
        return static_cast<u32>(encoded_edge);
    }

    u32 get_weight(u64 encoded_edge) {
        return static_cast<u32>(encoded_edge >> EDGE_BINARY_BUCKET_SIZE);
    }

    /**
     * Calculates MST of given graph and returns a ParallelArray<Edge> object
     */
    ParallelArray<Edge> calculate_mst(Graph graph, u32 NUM_THREADS = omp_get_max_threads()) {
        DSU node_sets(graph.num_nodes());
        ParallelArray<Edge> mst(graph.num_nodes() - 1);
        u32 current_mst_size = 0;
        u32 initial_num_nodes = graph.num_nodes();

        while (graph.num_nodes() != 1) {
            ParallelArray<atomic_u64> shortest_edges(initial_num_nodes);

            /* Calculating shortest edges from each node */
            #pragma omp parallel num_threads(NUM_THREADS)
            {
                std::unordered_map<u32, std::pair<u32, u32>> local_shortest_edges(graph.num_nodes());

                #pragma omp for
                for (u32 i = 0; i < initial_num_nodes; ++i) {
                    shortest_edges[i] = encode_edge(0, std::numeric_limits<u32>::max());
                }

                #pragma omp for
                for (u32 i = 0; i < graph.num_edges(); ++i) {
                    const Edge& e = graph.edges[i];

                    if (local_shortest_edges.count(e.from) == 0 ||
                        local_shortest_edges[e.from].first > e.weight) {

                        local_shortest_edges[e.from] = { e.weight, i };
                    }
                }

                for (const auto& p : local_shortest_edges) { /* O(M / p) operations in each thread */
                    u64 old = shortest_edges[p.first];

                    /* p.second = { weight, id } */
                    u64 encoded_edge = encode_edge(p.second.second, p.second.first);

                    while (true) { /* This loop is wait-free */
                        if (get_weight(old) < p.second.first ||
                            shortest_edges[p.first].compare_exchange_strong(old, encoded_edge)) {
                            break;
                        }
                    }
                }            
            }

            /* Calculating selected edges */
            ParallelArray<u32> edge_selected(graph.num_edges());

            #pragma omp parallel for
            for (u32 i = 0; i < graph.num_edges(); ++i) {
                edge_selected[i] = 0;
            }

            #pragma omp parallel for
            for (u32 i = 0; i < graph.num_nodes(); ++i) {
                u32 u = graph.nodes[i];
                const Edge& min_edge_u = graph.edges[get_id(shortest_edges[u])];

                u32 v = min_edge_u.to;
                const Edge& min_edge_v = graph.edges[get_id(shortest_edges[v])];
                
                if (min_edge_v.to != u || (min_edge_v.to == u && u < v)) {
                    node_sets.unite(u, v);
                    edge_selected[get_id(shortest_edges[u])] = 1;
                }
            }

            /* Adding edges to MST */
            PrefixSum edge_selected_prefix(graph.num_edges(), edge_selected);
            #pragma omp parallel for
            for (u32 i = 0; i < graph.num_edges(); ++i) {
                if (edge_selected[i]) {
                    mst[current_mst_size + edge_selected_prefix[i] - 1] = graph.edges[i];
                }
            }
            current_mst_size += edge_selected_prefix[graph.num_edges() - 1];

            /* Calculating remaining edges */
            ParallelArray<u32> edge_remains(graph.num_edges());
            #pragma omp parallel for
            for (u32 i = 0; i < graph.num_edges(); ++i) {
                edge_remains[i] = !node_sets.same_set(graph.edges[i].from, graph.edges[i].to);
            }

            PrefixSum edge_remains_prefix(graph.num_edges(), edge_remains);
            ParallelArray<Edge> new_edges(edge_remains_prefix[graph.num_edges() - 1]);
                
            #pragma omp parallel for
            for (u32 i = 0; i < graph.num_edges(); ++i) {
                if (edge_remains[i]) {
                    const Edge& old_edge = graph.edges[i];
                    new_edges[edge_remains_prefix[i] - 1] = Edge(node_sets.find_root(old_edge.from),
                                                                    node_sets.find_root(old_edge.to),
                                                                    old_edge.weight);
                }
            }
                
            /* Calculating remaining nodes */
            ParallelArray<u32> node_remains(graph.num_nodes());
            #pragma omp parallel for
            for (u32 i = 0; i < graph.num_nodes(); ++i) {
                node_remains[i] = (
                    node_sets.find_root(graph.nodes[i]) == graph.nodes[i]
                );
            }

            PrefixSum node_remains_prefix(graph.num_nodes(), node_remains);
            ParallelArray<u32> new_nodes(node_remains_prefix[graph.num_nodes() - 1]);

            #pragma omp parallel for
            for (u32 i = 0; i < graph.num_nodes(); ++i) {
                if (node_remains[i]) {
                    new_nodes[node_remains_prefix[i] - 1] = graph.nodes[i];
                }
            }

            /* Swapping old graph for new graph */
            graph.nodes.swap(new_nodes);
            graph.edges.swap(new_edges);
            graph.sort_edges();
        }

        return mst;
    }
};

/**
 * I used this struct for debugging purposes only, it is out of date
 * so you should never use it unless you're sure about it
 */
struct BoruvkaMST_verbose {
    /**
     * I use the same atomic pair that I use in dsu.h
     * TODO: create a special class for this
     */
    const u32 EDGE_BINARY_BUCKET_SIZE = 32;
    const u64 EDGE_WEIGHT_MASK = 0xFFFFFFFF00000000ULL;

    u64 encode_edge(u32 id, u32 weight) {
        return (static_cast<u64>(weight) << EDGE_BINARY_BUCKET_SIZE) + id;
    }

    u32 get_id(u64 encoded_edge) {
        return static_cast<u32>(encoded_edge);
    }

    u32 get_weight(u64 encoded_edge) {
        return static_cast<u32>(encoded_edge >> EDGE_BINARY_BUCKET_SIZE);
    }

    /**
     * Calculates MST of given graph and returns an EdgeSet object
     * TODO: Current implementation uses a sequential sort, I should write my own parallel sort.
     *       Also I don't want to use the default unordered_map because it sucks
     *       since my keys are <= Graph.num_nodes() I can use a simple array with an std::list
     */
    ParallelArray<Edge> calculate_mst(Graph graph, u32 NUM_THREADS = omp_get_max_threads()) {
        std::cout << graph.num_nodes() << " " << graph.num_edges() << "\n";
        for (auto e : graph.edges) {
            std::cout << e.from << " " << e.to << " " << e.weight << "\n";
        }

        DSU node_sets(graph.num_nodes());
        ParallelArray<Edge> mst(graph.num_nodes() - 1);
        u32 current_mst_size = 0;
        u32 initial_num_nodes = graph.num_nodes();

        u32 step_cnt = 0;

        while (graph.num_nodes() != 1) {
            std::cout << "Step " << ++step_cnt << "\n";

            ParallelArray<atomic_u64> shortest_edges(initial_num_nodes);

            /* Calculating shortest edges from each node */
            #pragma omp parallel num_threads(NUM_THREADS)
            {
                std::unordered_map<u32, std::pair<u32, u32>> local_shortest_edges(graph.num_nodes());
                #pragma omp for
                for (u32 i = 0; i < initial_num_nodes; ++i) {
                    shortest_edges[i] = encode_edge(0, std::numeric_limits<u32>::max());
                }

                #pragma omp single
                std::cout << "Initialized shortest edges\n";

                #pragma omp for
                for (u32 i = 0; i < graph.num_edges(); ++i) {
                    const Edge& e = graph.edges[i];

                    if (local_shortest_edges.count(e.from) == 0 ||
                        local_shortest_edges[e.from].first > e.weight) {

                        local_shortest_edges[e.from] = { e.weight, i };
                    }
                }

                #pragma omp single
                std::cout << "Calculated shortest edges in buckets\n";

                for (const auto& p : local_shortest_edges) { /* O(M / p) operations in each thread */
                    #pragma omp critical
                    std::cout << "from: " << p.first << " weight: " << p.second.first << " id: " << p.second.second  << "\n";

                    u64 old = shortest_edges[p.first];

                    /* p.second = { weight, id } */
                    u64 encoded_edge = encode_edge(p.second.second, p.second.first);

                    while (true) { /* This loop is wait-free */
                        if (get_weight(old) < p.second.first ||
                            shortest_edges[p.first].compare_exchange_strong(old, encoded_edge)) {
                            break;
                        }
                    }
                }            
            }
            std::cout << "Merged buckets\n";

            for (u32 i = 0; i < graph.num_nodes(); ++i) {
                std::cout << "node: " << graph.nodes[i] << "\n";
                std::cout << "mineid: " << get_id(shortest_edges[graph.nodes[i]]) << "\n";
                std::cout << "minew: " << get_weight(shortest_edges[graph.nodes[i]]) << "\n";
            }

            /* Calculating selected edges */
            ParallelArray<u32> edge_selected(graph.num_edges());

            #pragma omp parallel for
            for (u32 i = 0; i < graph.num_edges(); ++i) {
                edge_selected[i] = 0;
            }

            #pragma omp parallel for
            for (u32 i = 0; i < graph.num_nodes(); ++i) {
                u32 u = graph.nodes[i];
                const Edge& min_edge_u = graph.edges[get_id(shortest_edges[u])];

                u32 v = min_edge_u.to;

                #pragma omp critical
                std::cout << "u: " << u << " v: " << min_edge_u.to << " mu_id: " << get_id(shortest_edges[u]) << " mv_id: " << get_id(shortest_edges[v]) << "\n";

                const Edge& min_edge_v = graph.edges[get_id(shortest_edges[v])];
                
                if (min_edge_v.to != u || (min_edge_v.to == u && u < v)) {
                    node_sets.unite(u, v);
                    edge_selected[get_id(shortest_edges[u])] = 1;
                    #pragma omp critical
                    std:: cout << "selected " << get_id(shortest_edges[u]) << " from " << u << " to " << v << "\n";
                }
            }
            std::cout << "Calculated selected edges\n";

            /* Adding edges to MST */
            PrefixSum edge_selected_prefix(graph.num_edges(), edge_selected);
            #pragma omp parallel for
            for (u32 i = 0; i < graph.num_edges(); ++i) {
                if (edge_selected[i]) {
                    #pragma omp critical
                    std::cout << "Put edge no. " << i << " to position " << current_mst_size + edge_selected_prefix[i] - 1 << "\n";
                    mst[current_mst_size + edge_selected_prefix[i] - 1] = graph.edges[i];
                }
            }
            current_mst_size += edge_selected_prefix[graph.num_edges() - 1];
            std::cout << "Added mst edges\n";

            /* Calculating remaining edges */
            ParallelArray<u32> edge_remains(graph.num_edges());
            #pragma omp parallel for
            for (u32 i = 0; i < graph.num_edges(); ++i) {
                edge_remains[i] = !node_sets.same_set(graph.edges[i].from, graph.edges[i].to);
            }

            PrefixSum edge_remains_prefix(graph.num_edges(), edge_remains);
            ParallelArray<Edge> new_edges(edge_remains_prefix[graph.num_edges() - 1]);
                
            #pragma omp parallel for
            for (u32 i = 0; i < graph.num_edges(); ++i) {
                if (edge_remains[i]) {
                    const Edge& old_edge = graph.edges[i];
                    new_edges[edge_remains_prefix[i] - 1] = Edge(node_sets.find_root(old_edge.from),
                                                                    node_sets.find_root(old_edge.to),
                                                                    old_edge.weight);
                }
            }
            std::cout << "Created new edges\n";
                
            /* Calculating remaining nodes */
            ParallelArray<u32> node_remains(graph.num_nodes());
            #pragma omp parallel for
            for (u32 i = 0; i < graph.num_nodes(); ++i) {
                node_remains[i] = (
                    node_sets.find_root(graph.nodes[i]) == graph.nodes[i]
                );
            }

            PrefixSum node_remains_prefix(graph.num_nodes(), node_remains);
            ParallelArray<u32> new_nodes(node_remains_prefix[graph.num_nodes() - 1]);

            #pragma omp parallel for
            for (u32 i = 0; i < graph.num_nodes(); ++i) {
                if (node_remains[i]) {
                    new_nodes[node_remains_prefix[i] - 1] = graph.nodes[i];
                }
            }
            std::cout << "Created new nodes\n";

            /* Swapping old graph for new graph */
            graph.nodes.swap(new_nodes);
            graph.edges.swap(new_edges);
            graph.sort_edges();

            std::cout << "Num nodes: " << graph.num_nodes() << "\n";
            for (u32 i = 0; i < graph.num_nodes(); ++i) {
                std::cout << graph.nodes[i] << "\n";
            }

            std::cout << "Num edges: " << graph.num_edges() << "\n";
            for (u32 i = 0; i < graph.num_edges(); ++i) {
                std::cout << graph.edges[i].from << " " << graph.edges[i].to << "\n";
            }

            std::cout << "Swapped, sorted\n";
        }

        return mst;
    }
};

#endif
