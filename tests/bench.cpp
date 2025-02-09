#include <benchmark/benchmark.h>
#include <spath.hpp>
#include <vector>
#include <random>
#include <limits>

using Graph = std::vector<std::vector<std::pair<int, long long>>>; // first = to, second = weight

// graph generation
Graph generate_random_graph(int num_nodes, int num_edges, int min_weight, int max_weight) {
    Graph graph(num_nodes);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> node_dist(0, num_nodes - 1);
    std::uniform_int_distribution<> weight_dist(min_weight, max_weight);

    for (int i = 0; i < num_edges; ++i) {
        int u = node_dist(gen);
        int v = node_dist(gen);
        int weight = weight_dist(gen);
        if (u != v) {
            graph[u].push_back({v, weight});
            graph[v].push_back({u, weight}); // for undirected graph
        }
    }

    return graph;
}

static void BM_Dijkstra_high_density(benchmark::State& state) {
    int num_nodes = state.range(0);
    int num_edges = state.range(1);
    int min_weight = state.range(2);
    int max_weight = state.range(3);

    Graph graph = generate_random_graph(num_nodes, num_edges, min_weight, max_weight);
    Converter converter(graph);
    int start_node = 0;
    int end_node = num_nodes - 1;

    for (auto _ : state) {
        benchmark::DoNotOptimize(dijkstra_high_density(start_node, end_node, converter));
    }

    // For report
    state.counters["Nodes"] = num_nodes;
    state.counters["Edges"] = num_edges;
}

static void BM_Dijkstra_low_density(benchmark::State& state) {
    int num_nodes = state.range(0);
    int num_edges = state.range(1);
    int min_weight = state.range(2);
    int max_weight = state.range(3);

    Graph graph = generate_random_graph(num_nodes, num_edges, min_weight, max_weight);
    Converter converter(graph);
    int start_node = 0;
    int end_node = num_nodes - 1;

    for (auto _ : state) {
        benchmark::DoNotOptimize(dijkstra_low_density(start_node, end_node, converter));
    }

    // For report
    state.counters["Nodes"] = num_nodes;
    state.counters["Edges"] = num_edges;
}


BENCHMARK(BM_Dijkstra_high_density)
    ->Args({100, 500, 1, 100})    // 100 узлов, 500 рёбер, веса до 100
    ->Args({1000, 10000, 1, 100}) // 1000 узлов, 10000 рёбер, веса до 100
    ->Args({5000, 50000, 1, 100}) // 5000 узлов, 50000 рёбер, веса до 100
    ->Args({10000, 40000000, 1, 100}) // 10K узлов, 40'000'000 ребер 
    ->Unit(benchmark::kMillisecond);

BENCHMARK(BM_Dijkstra_low_density)
    ->Args({100, 500, 1, 100})    // 100 узлов, 500 рёбер, веса до 100
    ->Args({1000, 10000, 1, 100}) // 1000 узлов, 10000 рёбер, веса до 100
    ->Args({5000, 50000, 1, 100}) // 5000 узлов, 50000 рёбер, веса до 100
    ->Args({10000, 40000000, 1, 100}) // 10K узлов, 40'000'000 ребер
    ->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
