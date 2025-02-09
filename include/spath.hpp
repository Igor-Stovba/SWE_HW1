#ifndef SPATH_HPP_
#define SPATH_HPP_
#include <limits>
#include <utility>
#include <vector>
#include <set>
#include <cassert>
#include <algorithm>
#include <random>
#include <cstring>
#include <string>
#include <iostream>

/*
* alias Graph_ is shortcut for graph representation in algorithms and Converter class
*/
using Graph_ = std::vector<std::vector<std::pair<int, long long>>>; // first = dest, second = weight

/*
* Edge structure describes an edge in a graph. 
* source - vertex where an edge starts from
* dest - vertex where an edge points to
* weight - number. Our graphs are weighted
* INF is used in all algorithms as maximum possible value of weight in terms of limits of long long
*/
struct Edge {
    int source;
    int dest;
    long long weight;
};

struct Converter {
    /*
    * Converter transforms input graph representations to our uniform format
    * Possible input formats: adjacency list, adjacency matrix, list of edges
    * Output format: adjacency list (using Graph_)
    */
    Graph_ g; // Graph_ g is a result of convertions

    Graph_ convert(long long** matrix, int size1d) {
        /*
        * It is used if input was an adjacency matrix
        */
        Graph_ graph(size1d);
        for (int i = 0; i < size1d; i++) {
            for (int j = 0; j < size1d; j++) {
                // Loops aren't allowed + skip zeroes
                if (i == j or !matrix[i][j]) continue;
                graph[i].emplace_back(j, matrix[i][j]);
            }
        }
        return converted_graph;
    }
    
    std::vector<std::vector<std::pair<int,long long>>> 
    convert(const std::vector<Edge>& edge_list, bool is_directed) {
        int n = -1;
        for (auto& [source, dest, weight]: edge_list) 
            n = std::max(n, std::max(source, dest));
        if (++n == 0) return {};

        Graph_ graph(n);
        for (auto& [source, dest, weight]: edge_list) {
            graph[source].emplace_back(dest, weight);
            if (!is_directed) 
                graph[dest].emplace_back(source, weight);
        }
        return graph;
    }

    // Do nothing, input format = output format
    Converter(const Graph_& graph): g(graph) {}

    // Adjancency matrix
    Converter(long long** matrix, int size1d): g(convert(matrix, size1d)) {}

    // Edge list
    Converter(const std::vector<Edge>& edge_list, bool is_directed): g(convert(edge_list, is_directed)) {}
};

static std::pair<long long, std::vector<int>>
dijkstra_high_density(int s, int f, Converter c) {
    /*
    * Optimal for graphs with high density
    * Complexity: O(n^2 + m)
    */
    const Graph_ & g = c.g;
    int n = g.size();
    // s - start vertex, f - finish vertex
    assert(s >= 0 && s < n);
    assert(f >= 0 && f < n);

    long long INF = std::numeric_limits<long long>::max();
    std::vector<long long> dist(n, INF);
    std::vector<int> parent(n, -1);
    std::vector<bool> used(n, false);

    dist[start] = 0;
    for (int i = 0; i < n; i++) {
        int vertex = -1;
        for (int j = 0; j < n; j++) {
            if (!used[j] && (vertex == -1 || dist[j] < dist[vertex]))
                vertex = j;
        }
        if (dist[vertex] == INF) break;
        used[vertex] = true;

        for (int j = 0; j < g[vertex].size(); j++) {
            int u = g[vertex][j].first; // u is adjacent vertex to vertex
            long long weight = g[vertex][j].second;
            
            if (dist[u] > dist[vertex] + weight) {
                dist[u] = dist[vertex] + weight;
                parent[u] = vertex;
            }
        }
    }
    if (dist[finish] == INF) 
        return {-1, std::vector<int>()};

    std::vector<int> path {f};
    int tmp = f;
    while (parent[tmp] != s) {
        path.push_back(parent[tmp]);
        tmp = parent[tmp];
    }
    path.push_back(start);
    std::reverse(path.begin(), path.end());
    return {dist[finish], path};
}

static std::pair<long long, std::vector<int>>
dijkstra_low_density(int s, int f, Converter c) {
    /*
    * Optimal for graphs with low density
    * Complexity: O(m * log(n))
    */
    const Graph_ & g = c.g;
    int n = g.size();
    // s - start vertex, f - finish vertex
    assert(s >= 0 && s < n);
    assert(f >= 0 && f < n);

    long long INF = std::numeric_limits<long long>::max();
    std::vector<long long> dist(n, INF);
    std::vector<int> parent(n, -1);
    dist[s] = 0;

    std::set<std::pair<long long, int>> estimates;
    estimates.insert({0,s});

    while (!estimates.empty()) {
        auto [est, v] = *estimates.begin();
        estimates.erase(estimates.begin());
        for (auto& item: g[v]) {
            int to = item.first;
            long long weight = item.second;
            if (dist[to] > dist[v] + weight) {
                estimates.erase({dist[to], to});
                dist[to] = dist[v] + weight;
                parent[to] = v;
                estimates.insert({dist[to], to});
            }
        }
    }
    
    if (dist[f] == INF)
        return {-1, std::vector<int>()};
    std::vector<int> path = {f};
    int tmp = f;
    while (parent[tmp] != s) {
        path.push_back(parent[tmp]);
        tmp = parent[tmp];
    }
    path.push_back(start);
    std::reverse(path.begin(), path.end());
    return {dist[finish], path};
}


static std::vector<long long>
bellman_ford(int s, Converter c) {
    /*
    * Weight of every cycle should be non-negative
    * Complexity: O(mn)
    */
    const Graph_ & g = c.g;
    int n = g.size();
    // s - start vertex
    assert(s >= 0 && s < n);

    long long INF = std::numeric_limits<long long>::max();
    // O(n) memory
    std::vector<int> parent(n, -1);
    std::vector<long long> dist(n, INF);
    dist[s] = 0;

    for (int k = 1; k < n; k++) {
        bool changed = false; // Flag for early break

        for (int v = 0; v < n; v++) {
            for (auto& item : g[v]) {
                int u = item.first;
                long long weight = item.second;
                if (dist[u] != INF && dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    parent[v] = u;
                    changed = true;
                }
            }
        }

        if (!changed) break;
    }
    return dist;
}

static std::vector<long long>
johnson(Graph_ & g) {
    /*
    * Complexity: O(nm)
    * returns potentials!!!
    * After that you can apply more fast Dijkstra to find needed shortest paths.
    * It is well when you need to find multiple times the shortest path on a given graph
    */

    int n = g.size();

    /*
    * fakeVertex is used for creating one temp vertex which is needed 
    * for calculating potentials
    * After processing temp vertex will be removed
    */
    std::vector<std::pair<int, long long>> fakeVertex(n);
    for (int i = 0; i < n; i++)
        fakeVertex[i] = {i, 0};

    g.push_back(std::move(fakeVertex)); // adding temp vertex

    // Find potentials with Ford-Bellman
    std::vector<long long> potentials = bellman_ford(n, g);
    potentials.pop_back(); // deleting potential for temp vertex

    g.pop_back();  // deleting temp vertex
    return potentials; 
}

static std::pair<long long, std::vector<int>> 
astar(int s, int f, Converter c, long long (*heuristic)(int goal, int other)) {
    /*
    * Complexity: O(m*log(n)*complexity(heuristic))
    * modified Dijkstra's algorithm for searching path to a one specific point
    * by using some "good" estimator of distance to it for giving priority instead of pure distance 
    */
    const Graph_ & g = c.g;
    int n = g.size();
    // s - start vertex, f - finish vertex
    assert(s >= 0 && s < n);
    assert(f >= 0 && f < n);

    long long INF = std::numeric_limits<long long>::max();
    std::vector<long long>dist(n, INF);
    std::vector<long long>parent(n, -1);
    dist[s] = 0;
    std::set<std::pair<long long,long long>>estimates;
    estimates.insert({heuristic(f, s), s});
    while (!estimates.empty()) {
        auto [est, v] = *estimates.begin();
        if (v == f) break;
        estimates.erase(estimates.begin());
        for (auto& [neigh, weight]: g[v]) {
            if (dist[neigh] > dist[v]+weight) {
                dist[neigh] = dist[v]+weight;
                estimates.insert({dist[neigh]+heuristic(f, neigh), neigh});
                parent[neigh] = v;
            }
        }
    }
    if (dist[f] == INF) 
        return {-1, std::vector<int>()};
       
    std::vector<int>path;
    int tmp = finish;
    while (tmp != -1) {
        path.push_back(tmp);
        tmp = parent[tmp];
    }
    std::reverse(path.begin(), path.end());
    return {dist[finish], path};
}

static std::pair<std::vector<std::vector<int>>, std::vector<std::vector<long long>>> 
floyd_warshall(Converter c) {
    /*
    * Complexity: O(n^3)
    * computes distance from all to all and returns matrix next[u][v] which holds value for the next vertice on the path from u to v,
    * which can be then used in function path_from_next to restore any path.
    */
    const Graph_ & g = c.g;

    long long INF = std::numeric_limits<long long>::max();
    std::vector<std::vector<long long>>dist(g.size(), std::vector<long long>(g.size(), INF));
    std::vector<std::vector<int>>next(g.size(), std::vector<int>(g.size(), -1));
    for (int i = 0; i < g.size(); i++) {
        for (const auto&[v, weight]: g[i]) {
            dist[i][v] = weight;
            next[i][v] = v;
        }
    }
    for (int i = 0; i < g.size(); i++) {
        for (int u = 0; u < g.size(); u++) {
            for (int v = 0; v < g.size(); v++) {
                if (dist[u][i] != INF && dist[i][v] != INF) {
                    if (dist[u][i] + dist[i][v] < dist[u][v]) {
                        dist[u][v] = dist[u][i] + dist[i][v];
                        next[u][v] = next[u][i];
                    }
                }
            }
        }
    }
    return {next, dist};
}

static std::vector<int> 
path_from_next(const std::vector<std::vector<int>>& next, int s, int f) {
    /*
    * It is applied to 'next' from floyd_warshall algorithm to reconstruct the path
    * from start vertex (s) to finish vertex (f)
    */
    int cur = s;
    std::vector<int>path;
    while (cur != finish) {
        path.push_back(cur);
        cur = next[cur][finish];
    }

    path.push_back(f);
    return path;
}

#endif