# C++ library for finding the shortest paths

`spath.hpp` is a header-only C++ library for finding the shortest paths in graphs using various algorithms. The library provides efficient and versatile implementations suitable for different graph densities and use cases.

## Features

- **Dijkstra's Algorithm**  
  - Separate implementations for low-density (sparse) and high-density (dense) graphs.
- **Ford-Bellman Algorithm**  
  - Supports graphs with negative weight edges.
- **Johnson's Algorithm**  
  - Efficient for handling multiple queries for graphs with negative weights
- **Floyd-Warshall Algorithm**  
  - Comprehensive all-pairs shortest path solution for dense graphs.
- **A\* Algorithm**  
  - Optimized for pathfinding with heuristics in weighted graphs.
- **Lee Algorithm**  
  - BFS-like algorithm for pathfinding in maze-like graphs

## Time complexity

| Algorithm          | Best Use Case                         | Time Complexity            |
|--------------------|---------------------------------------|----------------------------|
| Dijkstra (sparse)  | Sparse graphs, non-negative weights   | O((V + E) * log V)         |
| Dijkstra (dense)   | Dense graphs, non-negative weights    | O(V^2)                     |
| Ford-Bellman       | Graphs with negative weights          | O(V * E)                   |
| Johnson            | Sparse graphs, all-pairs shortest paths | O(V^2 * log V + V * E) |
| Floyd-Warshall     | Dense graphs, all-pairs shortest paths | O(V^3)                 |
| A*                 | Pathfinding with heuristics           | O(m*log(n)*complexity(heuristic))       |
| Lee Algorithm       | Special-case optimizations            | O(rows*cols)                     |


## Installation

Simply include the `spath.hpp` file in your C++ project. The library is header-only, so no additional compilation or linking is required.

```cpp
#include "spath.hpp"
```

# Testing

We have tests.cpp with included header. On each push tests from this file will be run. The tests will use Google Test.

## License

This project is licensed under the MIT License.
  