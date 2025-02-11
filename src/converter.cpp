#include "converter.hpp"

    Graph_ Converter::convert(long long** matrix, int size1d) {
        /*
        * It is used if input was an adjacency matrix
        */
        Graph_ converted_graph(size1d);
        for (int i = 0; i < size1d; i++) {
            for (int j = 0; j < size1d; j++) {
                // Loops aren't allowed + skip zeroes
                if (i == j or !matrix[i][j]) continue;
                converted_graph[i].emplace_back(j, matrix[i][j]);
            }
        }
        return converted_graph;
    }
    
    std::vector<std::vector<std::pair<int,long long>>> 
    Converter::convert(const std::vector<Edge>& edge_list, bool is_directed) {
        int n = -1;
        for (auto& [source, dest, weight]: edge_list) 
            n = std::max(n, std::max(source, dest));
        if (++n == 0) return {};

        Graph_ converted_graph(n);
        for (auto& [source, dest, weight]: edge_list) {
            converted_graph[source].emplace_back(dest, weight);
            if (!is_directed) 
                converted_graph[dest].emplace_back(source, weight);
        }
        return converted_graph;
    }

    // Do nothing, input format = output format
    Converter::Converter(const Graph_& graph): graph(graph) {}

    // Adjancency matrix
    Converter::Converter(long long** matrix, int size1d): graph(convert(matrix, size1d)) {}

    // Edge list
    Converter::Converter(const std::vector<Edge>& edge_list, bool is_directed): graph(convert(edge_list, is_directed)) {}

