#ifndef __GRAPH_H
#define __GRAPH_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <omp.h>
#include <pair>
#include <random>
#include <string>
#include <tuple>

#include "defs.h"
#include "parallel_algorithms.h"
#include "parallel_array.h"
#include "parallel_random.h"
#include "utils.h"

struct Edge {
    u32 from;
    u32 to;
    u32 weight;

    Edge() {}

    Edge(u32 from, u32 to, u32 weight) : from(from), to(to), weight(weight) {}
};

bool operator<(const Edge& a, const Edge& b) {
    return std::tie(a.from, a.to, a.weight) < std::tie(b.from, b.to, b.weight);
}

struct Graph {
    ParallelArray<u32> nodes;
    ParallelArray<Edge> edges;

    Graph(u32 num_nodes, u32 num_edges) : nodes(num_nodes),
                                          edges(num_edges) {}

    u32 num_nodes() const {
        return nodes.size();
    }

    u32 num_edges() const {
        return edges.size();
    }

    void sort_edges() {
        parallel_sort(edges.begin(), edges.end());
    }
};

/**
 * TODO: use a parallel sort
 **/
Graph load_graph(std::string filename) {
    std::cout << "Loading graph from path " << filename << "\n";
    std::ifstream in(filename);

    u32 num_nodes;
    u32 num_edges;

    in >> num_nodes >> num_edges;

    std::cout << num_nodes << " nodes and " << num_edges << " edges\n";

    Graph G(num_nodes, num_edges * 2);
    RandomSequence R(num_edges);

    std::vector<std::pair<u32, u32>> edges_data;

    #pragma omp parallel for
    for (u32 i = 0; i < num_nodes; ++i) {
        G.nodes[i] = i;
    }

    for (u32 i = 0; i < num_edges; ++i) {
        u32 from, to;
        in >> from >> to;
        edges_data.push_back({ from, to });
    }

    for (u32 i = 0; i < num_edges; ++i) {
        G.edges[2 * i] = Edge(edges_data[i].first, edges_data[i].second, R[i]);
        G.edges[2 * i + 1] = Edge(edges_data[i].second, edges_data[i].first, R[i]);
    }

    G.sort_edges();

    std::cout << "Graph loaded\n";

    return G;
}

/**
 * Creates a random connected graph with n nodes and m edges
 * m >= n - 1
 */
Graph generate_graph(u32 n, u32 m) {
    Graph G(n, 2 * m);
    u32 cnt = 0;

    for (u32 i = 0; i < n; ++i) {
        G.nodes[i] = i;
    }

    for (u32 i = 1; i <= n - 1; ++i) {
        u32 weight = gen();
        u32 v = randint(0, i - 1);

        G.edges[cnt++] = Edge(i, v, weight);
        G.edges[cnt++] = Edge(v, i, weight);
    }

    for (u32 i = 1; i <= m - n + 1; ++i) {
        u32 weight = gen();
        u32 u = randint(0, n - 1);
        u32 v = randint(0, n - 2);
        if (v >= u) ++v;

        G.edges[cnt++] = Edge(u, v, weight);
        G.edges[cnt++] = Edge(v, u, weight);
    }

    G.sort_edges();

    return G;
}

void dfs(u32 u, std::vector<std::vector<u32>>& g, std::vector<u32>& used) {
    used[u] = 1;
    for (auto v : g[u]) {
        if (used[v] == 0) dfs(v, g, used);
    }
}

bool is_connected(Graph G) {
    std::vector<std::vector<u32>> g(G.num_edges());
    std::vector<u32> used(G.num_nodes());
    for (auto e : G.edges) {
        g[e.from].push_back(e.to);
        g[e.to].push_back(e.from);
    }

    u32 num_comp = 0;
    for (u32 i = 0; i < G.num_nodes(); ++i) {
        if (!used[i]) {
            dfs(i, g, used);
            ++num_comp;
        }
    }

    return num_comp == 1;
}

#endif
