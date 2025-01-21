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


using ll = long long;

std::pair<ll, std::vector<int>>
dijktsra_high_density(int s, int f,
const std::vector<std::vector<std::pair<int, ll>>>& g) {
    /*
    * Optimal for graphs with high density
    * Complexity: O(n^2 + m)
    */

    int n = g.size();
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
const std::vector<std::vector<std::pair<int, ll>>>& g) {
    /*
    * Optimal for graphs with low density
    * Complexity: O(m * log(n))
    */

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
        auto [est, v] = estimates.begin();
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
const std::vector<std::vector<std::pair<int, ll>>>& g) {
    /*
    * Weight of every cycle should be non-negative
    * Complexity: O(mn)
    */

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

#endif