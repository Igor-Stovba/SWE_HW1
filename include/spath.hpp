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
#include "edge.hpp"
#include "converter.hpp"

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

/*
* Optimal for graphs with high density
* Complexity: O(n^2 + m)
*/
std::pair<long long, std::vector<int>>
dijkstra_high_density(int start, int finish, Converter c);

/*
* Optimal for graphs with low density
* Complexity: O(m * log(n))
*/
std::pair<long long, std::vector<int>>
dijkstra_low_density(int start, int finish, Converter c);

/*
* Weight of every cycle should be non-negative
* Complexity: O(mn)
*/
std::pair<std::vector<long long>,std::vector<int>>
bellman_ford(int start, Converter c);

/*
* Complexity: O(nm)
* returns potentials!!!
* After that you can apply more fast Dijkstra to find needed shortest paths.
* It is well when you need to find multiple times the shortest path on a given graph
*/
std::vector<long long>
johnson(Graph_ & g);

/*
* Complexity: O(m*log(n)*complexity(heuristic))
* modified Dijkstra's algorithm for searching path to a one specific point
* by using some "good" estimator of distance to it for giving priority instead of pure distance 
*/
std::pair<long long, std::vector<int>> 
astar(int start, int finish, Converter c, long long (*heuristic)(int goal, int other));

/*
* Complexity: O(n^3)
* computes distance from all to all and returns matrix next[u][v] which holds value for the next vertice on the path from u to v,
* which can be then used in function path_from_next to restore any path.
*/
std::pair<std::vector<std::vector<int>>, std::vector<std::vector<long long>>> 
floyd_warshall(Converter c);

/*
* Helper function for restoring path from parent array which is returned by algorithm
* Note: parent[v] - previous vertex on the path from start to v
*/
std::vector<int>
path_from_parent(const std::vector<int>& parent, int start, int finish);

/*
* Helper function for restoring path from next array which is returned 
* by Floyd-Warshall algorithm
* Note: next[u][v] - next vertex on the path from u to v
*/
std::vector<int> 
path_from_next(const std::vector<std::vector<int>>& next, int start, int finish);

/*
* Helper function for using Ford-Bellman for 2 vertices 
* unlike from 1 to all in Ford-Bellman
*/
std::pair<long long, std::vector<int>>
bellman_for_two_vertices(int start, int finish, Converter c);

#endif