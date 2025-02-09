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

using Graph = std::vector<std::vector<std::pair<int, long long>>>; // first = to, second = weight

struct Edge {
    int from;
    int to;
    long long w;
};

struct Converter {
    Graph graph;

    Graph convert(long long** mtx, int n) {
        Graph converted_graph(n);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i == j or !mtx[i][j]) {
                    continue;
                }
                converted_graph[i].emplace_back(j, mtx[i][j]);
            }
        }
        return converted_graph;
    }
    
    const std::vector<std::vector<std::pair<int,long long>>> convert(std::vector<Edge>edge_list, bool is_directed) {
    int n = -1;
    for (auto&[a,b,c]:edge_list) {
        n = std::max(n, std::max(a,c));
    }
    n++;
    if (n == 0) {
        return std::vector<std::vector<std::pair<int,long long>>>();
    }
    Graph converted_graph(n);
    for (auto&[a,b,c]:edge_list) {
        converted_graph[a].emplace_back(c, b);
        if (!is_directed) {
            converted_graph[c].emplace_back(a,b);
        }
    }
    return converted_graph;
}

    Converter(const Graph& graph): graph(graph) {}
    Converter(long long** mtx, int n): graph(convert(mtx,n)) {}
    Converter(std::vector<Edge>edge_list, bool is_directed): graph(convert(edge_list, is_directed)) {}
};

std::pair<long long, std::vector<int>>
dijkstra_high_density(int start, int finish,
Converter c) {
    /*
    * Optimal for graphs with high density
    * Complexity: O(n^2 + m)
    */
    const Graph & g = c.g;
    int n = g.size();
    assert(start >= 0 && start < n);
    assert(finish >= 0 && finish < n);

    long long INF = std::numeric_limits<long long>::max();
    std::vector<long long> dist(n, INF);
    std::vector<int> p(n, -1);
    std::vector<bool> used(n, false);

    dist[start] = 0;
    for (int i = 0; i < n; i++) {
        int v = -1;
        for (int j = 0; j < n; j++) {
            if (!used[j] && (v == -1 || dist[j] < dist[v]))
                v = j;
        }
        if (dist[v] == INF) break;
        used[v] = true;

        for (int j = 0; j < g[v].size(); j++) {
            int u = g[v][j].first;
            long long weight = g[v][j].second;
            
            if (dist[u] > dist[v] + weight) {
                dist[u] = dist[v] + weight;
                p[u] = v;
            }
        }
    }
    if (dist[finish] == INF) 
        return {-1, std::vector<int>()};
    std::vector<int> path = {finish};
    int tmp = finish;
    while (p[tmp] != start) {
        path.push_back(p[tmp]);
        tmp = p[tmp];
    }
    path.push_back(start);
    std::reverse(path.begin(), path.end());
    return {dist[finish], path};
}

std::pair<long long, std::vector<int>>
dijkstra_low_density(int start, int finish,
Converter c) {
    /*
    * Optimal for graphs with low density
    * Complexity: O(m * log(n))
    */
    const Graph & g = c.g;
    int n = g.size();
    assert(start >= 0 && start < n);
    assert(finish >= 0 && finish < n);

    long long INF = std::numeric_limits<long long>::max();
    std::vector<long long> dist(n, INF);
    std::vector<int> p(n, -1);
    dist[start] = 0;

    std::set<std::pair<long long, int>> estimates;
    estimates.insert({0,start});

    while (!estimates.empty()) {
        auto [est, v] = *estimates.begin();
        estimates.erase(estimates.begin());
        for (auto& item: g[v]) {
            int to = item.first;
            long long weight = item.second;
            if (dist[to] > dist[v] + weight) {
                estimates.erase({dist[to], to});
                dist[to] = dist[v] + weight;
                p[to] = v;
                estimates.insert({dist[to], to});
            }
        }
    }
    

    if (dist[finish] == INF)
        return {-1, std::vector<int>()};
    std::vector<int> path = {finish};
    int tmp = finish;
    while (p[tmp] != start) {
        path.push_back(p[tmp]);
        tmp = p[tmp];
    }
    path.push_back(start);
    std::reverse(path.begin(), path.end());
    return {dist[finish], path};
}


std::vector<long long>
bellman_ford(int start,
Converter c) {
    /*
    * Weight of every cycle should be non-negative
    * Complexity: O(mn)
    */
    const Graph & g = c.g;
    int n = g.size();
    assert(start >= 0 && start < n);

    long long INF = std::numeric_limits<long long>::max();
    std::vector<int> p(n, -1);

    // O(n) memory
    std::vector<long long> prev(n, INF), curr(n, INF);
    prev[start] = 0;

    for (int k = 1; k < n; k++) {
        curr = prev; 

        for (int v = 0; v < n; v++) {
            for (auto& item : g[v]) {
                int u = item.first;
                long long weight = item.second;
                if (prev[u] != INF && prev[u] + weight < curr[v]) {
                    p[v] = u;
                    curr[v] = prev[u] + weight; 
                }
            }
        }
        prev = curr;
    }

    return curr;
}

std::vector<long long>
johnson(Graph & g) {
    /*
    * Complexity: O(nm)
    * returns potentials!!!
    * After that you can apply more fast Dijkstra to find needed shortest paths.
    * It is well when you need to find multiple times the shortest path on a given graph
    */

    int n = g.size();

    std::vector<std::pair<int, long long>> fakeVertex(n);
    for (int i = 0; i < n; i++)
        fakeVertex[i] = {i, 0};

    g.push_back(std::move(fakeVertex));

    // Find potentials with Ford-Bellman
    std::vector<long long> potentials = bellman_ford(n, g);
    potentials.pop_back(); // Delete fake vertex

    g.pop_back();   
    return potentials; 
}

std::pair<long long, std::vector<int>> astar(int start, int finish, Converter c, long long (*heuristic)(int goal, int other)) {
    /*
    * Complexity: O(m*log(n)*complexity(heuristic))
    * modified Dijkstra's algorithm for searching path to a one specific point
    * by using some "good" estimator of distance to it for giving priority instead of pure distance 
    */
    const Graph & g = c.g;
    int n = g.size();

    std::vector<long long>dist(n, __LONG_LONG_MAX__);
    std::vector<long long>p(n, -1);
    dist[start] = 0;
    std::set<std::pair<long long,long long>>estimates;
    estimates.insert({heuristic(finish, start), start});
    while (!estimates.empty()) {
        auto [est, v] = *estimates.begin();
        if (v == finish) {
            break;
        }
        estimates.erase(estimates.begin());
        for (auto& [neigh, weight]: g[v]) {
            if (dist[neigh] > dist[v]+weight) {
                dist[neigh] = dist[v]+weight;
                estimates.insert({dist[neigh]+heuristic(finish, neigh), neigh});
                p[neigh] = v;
            }
        }
    }
    if (dist[finish] == __LONG_LONG_MAX__) {
        return {-1, std::vector<int>()};
    }    
    std::vector<int>path;
    int tmp = finish;
    while (tmp != -1) {
        path.push_back(tmp);
        tmp = p[tmp];
    }
    std::reverse(path.begin(), path.end());
    return {dist[finish], path};
}

std::pair<std::vector<std::vector<int>>, std::vector<std::vector<long long>>> floyd_warshall(Converter c) {
    /*
    * Complexity: O(n^3)
    * computes distance from all to all and returns matrix next[u][v] which holds value for the next vertice on the path from u to v,
    * which can be then used in function path_from_next to restore any path.
    */
    const Graph & g = c.g;
    std::vector<std::vector<long long>>dist(g.size(), std::vector<long long>(g.size(), __LONG_LONG_MAX__));
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
                if (dist[u][i] != __LONG_LONG_MAX__ && dist[i][v] != __LONG_LONG_MAX__) {
                    if (dist[u][i]+dist[i][v] < dist[u][v]) {
                        dist[u][v] = dist[u][i]+dist[i][v];
                        next[u][v] = next[u][i];
                    }
                }
            }
        }
    }
    return {next, dist};
}

std::vector<int> path_from_next(const std::vector<std::vector<int>>& next, int start, int finish) {
    int cur = start;
    std::vector<int>path;
    while (cur != finish) {
        path.push_back(cur);
        cur = next[cur][finish];
    }
    path.push_back(finish);
    return path;
}

#endif