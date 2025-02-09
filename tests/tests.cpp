#include "spath.hpp"
#include <iostream>
#include <catch2/catch_test_macros.hpp>

void test_shortest_path(Converter adjacency_list_converter, Converter adjacency_matrix_converter, Converter edge_list_converter, int s, int f,
(std::pair<long long, std::vector<int>>)(*algorithm)()
std::pair<long long, std::vector<int>> expected) {
    auto res1 = dijkstra_high_density(s,f,adjacency_list_converter);
    auto res2 = dijkstra_high_density(s,f,adjacency_matrix_converter);
    auto res3 = dijkstra_high_density(s,f,edge_list_converter);

    auto res12 = dijkstra_low_density(s,f,adjacency_list_converter);
    auto res22 = dijkstra_low_density(s,f,adjacency_matrix_converter);
    auto res32 = dijkstra_low_density(s,f,edge_list_converter);

    REQUIRE( res1 == expected );
    REQUIRE( res2 == expected );
    REQUIRE( res3 == expected );

    REQUIRE( res12 == expected );
    REQUIRE( res22 == expected );
    REQUIRE( res32 == expected );
}

TEST_CASE( "Dijkstra correctness", "[Requirement 1]" ) {
    SECTION("empty graph") {
        int n = 5;
        std::vector<std::vector<std::pair<int, long long>>>adjacency_list(n);
        long long** adjacency_matrix = new long long*[n];
        for (int i = 0; i < n; i++) {
            adjacency_matrix[i] = new long long[n];
            for (int j = 0; j < n; j++) {
                adjacency_matrix[i][j] = 0;
            }
        }
        std::vector<Edge>edge_list;

        Converter adjacency_list_converter = Converter(adjacency_list);
        Converter adjacency_matrix_converter = Converter(adjacency_matrix, 5);

        auto res1 = dijkstra_high_density(0,2,adjacency_list_converter);
        auto res2 = dijkstra_high_density(0,2,adjacency_matrix_converter);

        auto res12 = dijkstra_low_density(0,2,adjacency_list_converter);
        auto res22 = dijkstra_low_density(0,2,adjacency_matrix_converter);


        std::pair<long long, std::vector<int>> expected = {-1,std::vector<int>()};

        REQUIRE( res1 == expected );
        REQUIRE( res2 == expected );
        REQUIRE( res12 == expected );
        REQUIRE( res22 == expected );

        for (int i = 0; i < n; i++) {
            delete adjacency_matrix[i];
        }
        delete adjacency_matrix;
    }

    SECTION("full graph") {
        int n = 5;
        std::vector<std::vector<std::pair<int, long long>>>adjacency_list(n);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i == j) {
                    continue;
                }
                adjacency_list[i].emplace_back(j, 1);
            }
        }

        long long** adjacency_matrix = new long long*[n];
        for (int i = 0; i < n; i++) {
            adjacency_matrix[i] = new long long[n];
            for (int j = 0; j < n; j++) {
                if (i == j) {
                    continue;
                }
                adjacency_matrix[i][j] = 1;
            }
        }
        std::vector<Edge>edge_list;

        for (int i = 0; i < n; i++) {
            for (int j = i+1; j < n; j++) {
                if (i == j) {
                    continue;
                }
                Edge e = {i, 1, j};
                edge_list.push_back(e);
            }
        }

        Converter adjacency_list_converter = Converter(adjacency_list);
        Converter adjacency_matrix_converter = Converter(adjacency_matrix, 5);
        Converter edge_list_converter = Converter(edge_list, false);
        std::pair<long long, std::vector<int>> expected = {1,{0,3}};

        test_dijkstra(adjacency_list_converter, adjacency_matrix_converter, edge_list_converter, 0, 3, expected);

        for (int i = 0; i < n; i++) {
            delete adjacency_matrix[i];
        }
        delete adjacency_matrix;

    }
    SECTION("complex graph") {
        int n = 5;
        std::vector<std::vector<std::pair<int, long long>>>adjacency_list(n);
        adjacency_list[0] = {{1,100}, {4, 10}, {3, 20}};
        adjacency_list[1] = {{2,7}, {0,100}};
        adjacency_list[2] = {{1,7}, {3,20}};
        adjacency_list[3] = {{4,15},{0,20},{2,20}};
        adjacency_list[4] = {{0,10},{3,15}};

        long long** adjacency_matrix = new long long*[n];
        for (int i = 0; i < n; i++) {
            adjacency_matrix[i] = new long long[n];
            for (int j = 0; j < n; j++) {
                adjacency_matrix[i][j] = 0;
            }
        }
        adjacency_matrix[0][1] = adjacency_matrix[1][0] = 100;
        adjacency_matrix[0][4] = adjacency_matrix[4][0] = 10;
        adjacency_matrix[0][3] = adjacency_matrix[3][0] = 20;
        adjacency_matrix[1][2] = adjacency_matrix[2][1] = 7;
        adjacency_matrix[2][3] = adjacency_matrix[3][2] = 20;
        adjacency_matrix[3][4] = adjacency_matrix[4][3] = 15;

        std::vector<Edge>edge_list = {{0,100,1},{0,10,4},{0,20,3},{1,7,2},{2,20,3},{3,15,4}};

        Converter adjacency_list_converter = Converter(adjacency_list);
        Converter adjacency_matrix_converter = Converter(adjacency_matrix, 5);
        Converter edge_list_converter = Converter(edge_list, false);

        std::pair<long long, std::vector<int>> expected = {47,{0,3,2,1}};


        test_dijkstra(adjacency_list_converter, adjacency_matrix_converter, edge_list_converter, 0, 1, expected);

        for (int i = 0; i < n; i++) {
            delete adjacency_matrix[i];
        }
        delete adjacency_matrix;


    }

    SECTION("two components") {
        int n = 5;
        std::vector<std::vector<std::pair<int, long long>>>adjacency_list(n);
        adjacency_list[0] = {{4,10}, {3,20}};
        adjacency_list[1] = {{2,7}};
        adjacency_list[2] = {{1,7}};
        adjacency_list[3] = {{0,20},{4,15}};
        adjacency_list[4] = {{0,10}, {3,15}};

        long long** adjacency_matrix = new long long*[n];
        for (int i = 0; i < n; i++) {
            adjacency_matrix[i] = new long long[n];
            for (int j = 0; j < n; j++) {
                adjacency_matrix[i][j] = 0;
            }
        }
        adjacency_matrix[0][3] = adjacency_matrix[3][0] = 20;
        adjacency_matrix[0][4] = adjacency_matrix[4][0] = 10;
        adjacency_matrix[1][2] = adjacency_matrix[2][1] = 7;
        adjacency_matrix[3][4] = adjacency_matrix[4][3] = 15;

        std::vector<Edge>edge_list = {{0,20,3},{0,10,4},{1,7,2},{3,15,4}};
        Converter adjacency_list_converter = Converter(adjacency_list);
        Converter adjacency_matrix_converter = Converter(adjacency_matrix, 5);
        Converter edge_list_converter = Converter(edge_list, false);
        std::pair<long long, std::vector<int>> expected = {-1,std::vector<int>()};

        test_dijkstra(adjacency_list_converter, adjacency_matrix_converter, edge_list_converter, 2, 4, expected);

        for (int i = 0; i < n; i++) {
            delete adjacency_matrix[i];
        }
        delete adjacency_matrix;
    }
       
}