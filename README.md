# C++ library for finding the shortest paths

Spath is a static C++ library for finding the shortest paths in graphs using various algorithms. The library provides efficient and versatile implementations suitable for different graph densities and use cases.

## Features

- **Dijkstra's Algorithm (Low density version)**  
  - Optimized for graphs with low density
- **Dijkstra's Algorithm (High density version)**  
  - Optimized for graphs with high density
- **Ford-Bellman Algorithm**  
  - Supports graphs with negative weight edges.
- **Johnson's Algorithm**  
  - Efficient for handling multiple queries for graphs with negative weights
- **Floyd-Warshall Algorithm**  
  - Comprehensive all-pairs shortest path solution for dense graphs.
- **A\* Algorithm**  
  - Optimized for pathfinding with heuristics in weighted graphs.

## Time complexity

| Algorithm          | Best Use Case                         | Time Complexity            |
|--------------------|---------------------------------------|----------------------------|
| Dijkstra (sparse)  | Sparse graphs, non-negative weights   | O((V + E) * log V)         |
| Dijkstra (dense)   | Dense graphs, non-negative weights    | O(V^2)                     |
| Ford-Bellman       | Graphs with negative weights          | O(V * E)                   |
| Johnson            | Sparse graphs, all-pairs shortest paths | O(V^2 * log V + V * E) |
| Floyd-Warshall     | Dense graphs, all-pairs shortest paths | O(V^3)                 |
| A*                 | Pathfinding with heuristics           | O(m*log(n)*complexity(heuristic))       |


## Installation

Clone our library. then build and install with cmake

```
mkdir build
cd build
cmake ..
cmake --build . --target Spath
cmake --install .
```
This will add our library to your system path. After that you can use it like:

```cpp
#include <spath/spath.hpp>
```

# Testing

We have `tests.cpp` with included header. On each push tests from this file will be run. The tests will use Catch2 framework.
We benchmarked our algorithms (`bench.cpp`) with help of Google Benchmark. You can see results in Github Actions. 

## License

This project is licensed under the MIT License.
  
