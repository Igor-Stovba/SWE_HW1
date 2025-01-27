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


using ll = long long;

struct Edge {
    int from;
    ll w;
    int to;
};

struct Converter {
    const std::vector<std::vector<std::pair<int, ll>>>& g;

    const std::vector<std::vector<std::pair<int, ll>>> convert(ll** mtx, int n) {
        std::vector<std::vector<std::pair<int, ll>>>graph(n);
        std::cout << graph.size() << " SIZE!!";
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i == j or !mtx[i][j]) {
                    continue;
                }
                graph[i].emplace_back(j, mtx[i][j]);
            }
        }
        return graph;
    }
    
    const std::vector<std::vector<std::pair<int,ll>>> convert(std::vector<Edge>edge_list) {
    int n = -1;
    for (auto&[a,b,c]:edge_list) {
        n = std::max(n, a);
    }
    n++;
    if (n == 0) {
        return std::vector<std::vector<std::pair<int,ll>>>();
    }
    std::vector<std::vector<std::pair<int, ll>>>g(n);
    for (auto&[a,b,c]:edge_list) {
        g[a].emplace_back(c, b);
    }
    return g;
}

    Converter(const std::vector<std::vector<std::pair<int, ll>>>& graph): g(graph) {}
    Converter(ll** mtx, int n): g(convert(mtx,n)) {}
    Converter(std::vector<Edge>edge_list): g(convert(edge_list)) {}
};

std::pair<ll, std::vector<int>>
dijktsra_high_density(int s, int f,
Converter c) {
    /*
    * Optimal for graphs with high density
    * Complexity: O(n^2 + m)
    */
    const std::vector<std::vector<std::pair<int, ll>>>& g = c.g;
    int n = g.size();
    std::cout << n << " SIZE Last";
    assert(s >= 0 && s < n);
    assert(f >= 0 && f < n);

    ll INF = std::numeric_limits<ll>::max();
    std::vector<ll> dist(n, INF);
    std::vector<int> p(n, -1);
    std::vector<bool> used(n, false);

    dist[s] = 0;
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
            ll weight = g[v][j].second;
            
            if (dist[u] > dist[v] + weight) {
                dist[u] = dist[v] + weight;
                p[u] = v;
            }
        }
    }
    if (dist[f] == INF) 
        return {-1, std::vector<int>()};
    std::vector<int> path = {f};
    int tmp = f;
    while (p[tmp] != s) {
        path.push_back(p[tmp]);
        tmp = p[tmp];
    }
    path.push_back(s);
    std::reverse(path.begin(), path.end());
    return {dist[f], path};
}

std::pair<ll, std::vector<int>>
dijktsra_low_density(int s, int f,
Converter c) {
    /*
    * Optimal for graphs with low density
    * Complexity: O(m * log(n))
    */
    const std::vector<std::vector<std::pair<int, ll>>>& g = c.g;
    int n = g.size();
    assert(s >= 0 && s < n);
    assert(f >= 0 && f < n);

    ll INF = std::numeric_limits<ll>::max();
    std::vector<ll> dist(n, INF);
    std::vector<int> p(n, -1);
    dist[s] = 0;

    std::set<std::pair<ll, int>> estimates;
    for (int i = 0; i < n; i++) {
        estimates.insert({dist[i], i});
    }

    while (!estimates.empty()) {
        auto [est, v] = *estimates.begin();
        estimates.erase(estimates.begin());
        for (auto& item: g[v]) {
            int to = item.first;
            ll weight = item.second;
            if (dist[to] > dist[v] + weight) {
                estimates.erase({dist[to], to});
                dist[to] = dist[v] + weight;
                p[to] = v;
                estimates.insert({dist[to], to});
            }
        }
    }

    if (dist[f] == INF)
        return {-1, std::vector<int>()};
    std::vector<int> path = {f};
    int tmp = f;
    while (p[tmp] != s) {
        path.push_back(p[tmp]);
        tmp = p[tmp];
    }
    path.push_back(s);
    std::reverse(path.begin(), path.end());
    return {dist[f], path};
}


std::vector<ll>
bellman_ford(int s,
Converter c) {
    /*
    * Weight of every cycle should be non-negative
    * Complexity: O(mn)
    */
    const std::vector<std::vector<std::pair<int, ll>>>& g = c.g;
    int n = g.size();
    assert(s >= 0 && s < n);

    ll INF = std::numeric_limits<ll>::max();
    std::vector<int> p(n, -1);

    // O(n) memory
    std::vector<ll> prev(n, INF), curr(n, INF);
    prev[s] = 0;

    for (int k = 1; k < n; k++) {
        curr = prev; 

        for (int v = 0; v < n; v++) {
            for (auto& item : g[v]) {
                int u = item.first;
                ll weight = item.second;
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

std::vector<ll>
johnson(std::vector<std::vector<std::pair<int, ll>>>& g) {
    /*
    * Complexity: O(nm)
    * returns potentials!!!
    * After that you can apply more fast Dijkstra to find needed shortest paths.
    * It is well when you need to find multiple times the shortest path on a given graph
    */

    int n = g.size();

    std::vector<std::pair<int, ll>> fakeVertex(n);
    for (int i = 0; i < n; i++)
        fakeVertex[i] = {i, 0};

    g.push_back(std::move(fakeVertex));

    // Find potentials with Ford-Bellman
    std::vector<ll> potentials = bellman_ford(n, g);
    potentials.pop_back(); // Delete fake vertex

    g.pop_back();   
    return potentials; 
}

std::pair<ll, std::vector<int>> astar(int s, int f, Converter c, ll (*heuristic)(int goal, int other)) {
    /*
    * Complexity: O(m*log(n)*complexity(heuristic))
    * modified Dijkstra's algorithm for searching path to a one specific point
    * by using some "good" estimator of distance to it for giving priority instead of pure distance 
    */
    const std::vector<std::vector<std::pair<int, ll>>>& g = c.g;
    int n = g.size();

    std::vector<ll>dist(n, __LONG_LONG_MAX__);
    std::vector<ll>p(n, -1);
    dist[s] = 0;
    std::set<std::pair<ll,ll>>estimates;
    estimates.insert({heuristic(f, s), s});
    while (!estimates.empty()) {
        auto [est, v] = *estimates.begin();
        if (v == f) {
            break;
        }
        estimates.erase(estimates.begin());
        for (auto& [neigh, weight]: g[v]) {
            if (dist[neigh] > dist[v]+weight) {
                dist[neigh] = dist[v]+weight;
                estimates.insert({dist[neigh]+heuristic(f, neigh), neigh});
                p[neigh] = v;
            }
        }
    }
    if (dist[f] == __LONG_LONG_MAX__) {
        return {-1, std::vector<int>()};
    }    
    std::vector<int>path;
    int tmp = f;
    while (tmp != -1) {
        path.push_back(tmp);
        tmp = p[tmp];
    }
    std::reverse(path.begin(), path.end());
    return {dist[f], path};
}

std::pair<std::vector<std::vector<int>>, std::vector<std::vector<ll>>> floyd_warshall(Converter c) {
    /*
    * Complexity: O(n^3)
    * computes distance from all to all and returns matrix next[u][v] which holds value for the next vertice on the path from u to v,
    * which can be then used in function path_from_next to restore any path.
    */
    const std::vector<std::vector<std::pair<int, ll>>>& g = c.g;
    std::vector<std::vector<ll>>dist(g.size(), std::vector<ll>(g.size(), __LONG_LONG_MAX__));
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

std::vector<int> path_from_next(const std::vector<std::vector<int>>& next, int s, int f) {
    int cur = s;
    std::vector<int>path;
    while (cur != f) {
        path.push_back(cur);
        cur = next[cur][f];
    }
    path.push_back(f);
    return path;
}



std::pair<ll, std::vector<std::pair<int,int>>> lee(bool **maze, int rows, int cols, std::pair<int,int>s , std::pair<int,int> f) {
    /*
    * Complexity: O(rows*cols)
    * Searches the shortest path in a rectangular maze in a bfs manner
    * returns the distance and the path.
    */
    
    bool **used = new bool*[rows];
    ll **dist = new ll*[rows];
    for (int i = 0; i < rows; i++) {
        used[i] = new bool[cols];
        dist[i] = new ll[cols];
        std::memcpy(used[i], maze[i], sizeof(maze[i][0]) * cols);
        std::memset(dist[i], -1, cols);
    }
    std::vector<std::pair<int,int>>wave{s};
    std::vector<std::pair<int,int>>path;
    ll d = 1;
    used[s.first][s.second] = true;
    const int deltas[] = {-1, 0, 1};
    do
    {
        std::vector<std::pair<int,int>>new_wave;
        for (const auto& [i,j]: wave) {
            for (int a = 0; a < 3; a++) {
                for (int b = 0; b < 3; b++) {
                    if (0 <= i+a && i+a < rows && 0 <= j+b && j+b < cols && !used[i+a][j+b]) {
                        used[i+a][j+b] = true;
                        dist[i+a][j+b] = d;
                        new_wave.emplace_back(i+a, j+b);
                    }
                }
            }
        }
        d++;
        wave = std::move(new_wave);
    } while (!wave.empty() && dist[f.first][f.second] == -1);
    if (dist[f.first][f.second] == -1) {
        path = std::vector<std::pair<int,int>>();
    } else {
        ll cur_d = dist[f.first][f.second];
        auto tmp = f;
        while (tmp != s) {
            path.push_back(tmp);
            auto[i,j] = tmp;
            bool flag = false;
            for (int a = 0; a < 3 && !flag; a++) {
                for (int b = 0; b < 3 && !flag; b++) {
                    if (0 <= i+a && i+a < rows && 0 <= j+b && j+b < cols && dist[i+a][j+b] == cur_d - 1) {
                        tmp = {i+a,j+b};
                        cur_d--;
                        flag = true;
                    }
                }
            }
        }
        path.push_back(s);
        std::reverse(path.begin(), path.end());
    }
    ll ans = dist[f.first][f.second];
    for (int i = 0; i < rows; i++) {
        delete used[i];
        delete dist[i];
    }
    delete used;
    delete dist;
    return {ans, path};
}

bool** generate_maze(int rows, int cols, double p = 0.5, std::string seed = "0") {
    /*
    generate random maze with p-frequency of obstacles
    */
    std::mt19937_64 rng;
    std::seed_seq seed1 (seed.begin(),seed.end());
    rng.seed(seed1);
    std::uniform_real_distribution<double>unif(0,1);
    bool** maze = new bool*[rows];
    for (int i = 0; i < rows; i++) {
        maze[i] = new bool[cols];
        for (int j = 0; j < cols; j++) {
            maze[i][j] = (unif(rng) > p);
        }
    }
    return maze;
}

void free_maze(bool** maze, int rows) {
    /*
    free memory from generate_maze
    */
    for (int i = 0; i < rows; i++) {
        delete maze[i];
    }
    delete maze;
}

void print_maze_path(bool** maze, int rows, int cols, const std::vector<std::pair<int, int>>& path) {
    /*
    print a maze path as ascii
    */
    char** map = new char*[rows];
    for (int i = 0; i < rows; i++) {
        map[i] = new char[cols];
        for (int j = 0; j < cols; j++) {
            map[i][j] = maze[i][j] ? 'X' : '.';
        }
    }
    for (auto&[i,j]: path) {
        map[i][j] = 'O';
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%c ", map[i][j]);
        }
        printf("\n");
    }
    for (int i = 0; i < cols; i++) {
        delete map[i];
    }
    delete map;
}

std::vector<std::vector<std::pair<int, ll>>> generate_graph(int n, double p = 0.5, std::string seed = "0") {
    /*
    generate random graph with p-frequency of edges
    */
    std::mt19937_64 rng;
    std::seed_seq seed1 (seed.begin(),seed.end());
    rng.seed(seed1);
    std::uniform_real_distribution<double>unif(0,1);
    std::uniform_int_distribution<ll>weights(0, 100);
    std::vector<std::vector<std::pair<int,ll>>> g(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                continue;
            }
            double coin = unif(rng);
            ll weight = weights(rng);
            if (coin > p) {
                g[i].emplace_back(j, weight);
            }
        }
    }
    return g;
}

#endif