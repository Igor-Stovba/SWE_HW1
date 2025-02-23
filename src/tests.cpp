#include <spath/spath.hpp>
#include <iostream>
#include <catch2/catch_test_macros.hpp>

void test_shortest_path(Converter adjacency_list_converter, Converter adjacency_matrix_converter, Converter edge_list_converter, 
int start, int finish, std::pair<long long, std::vector<int>> expected, 
std::pair<long long, std::vector<int>> (*algorithm)(int start, int finish , Converter converter)) {
    auto res1 = algorithm(start,finish,adjacency_list_converter);
    auto res2 = algorithm(start,finish,adjacency_matrix_converter);
    auto res3 = algorithm(start,finish,edge_list_converter);
    REQUIRE( res1 == expected );
    REQUIRE( res2 == expected );
    REQUIRE( res3 == expected );
}

TEST_CASE("Converter", "[Converter]") {
    SECTION("adjacency list") {
        int n = 5;
        Graph_ adjacency_list(n);
        adjacency_list[0] = {{1, 10}, {2, 20}, {4, 50}};
        adjacency_list[1] = {{0, 10}, {3, 30}};
        adjacency_list[2] = {{0, 20}, {3, 40}, {4, 60}};
        adjacency_list[3] = {{1, 30}, {2, 40}, {4, 70}};
        adjacency_list[4] = {{0, 50}, {2, 60}, {3, 70}};

        Converter converter(adjacency_list);
        REQUIRE(converter.graph == adjacency_list);
    }

    SECTION("adjacency matrix") {
        int n = 5;
        long long** adjacency_matrix = new long long*[n];
        for (int i = 0; i < n; i++) {
            adjacency_matrix[i] = new long long[n]{};
        }

        adjacency_matrix[0][1] = 10;
        adjacency_matrix[0][2] = 20;
        adjacency_matrix[0][4] = 50;
        adjacency_matrix[1][3] = 30;
        adjacency_matrix[2][3] = 40;
        adjacency_matrix[2][4] = 60;
        adjacency_matrix[3][4] = 70;
        adjacency_matrix[1][0] = 10;
        adjacency_matrix[2][0] = 20;
        adjacency_matrix[3][1] = 30;
        adjacency_matrix[3][2] = 40;
        adjacency_matrix[4][0] = 50;
        adjacency_matrix[4][2] = 60;
        adjacency_matrix[4][3] = 70;

        Converter converter(adjacency_matrix, n);
        Graph_ expected = {
            {{1, 10}, {2, 20}, {4, 50}},
            {{0, 10}, {3, 30}},
            {{0, 20}, {3, 40}, {4, 60}},
            {{1, 30}, {2, 40}, {4, 70}},
            {{0, 50}, {2, 60}, {3, 70}}
        };

        REQUIRE(converter.graph == expected);

        for (int i = 0; i < n; i++) {
            delete[] adjacency_matrix[i];
        }
        delete[] adjacency_matrix;
    }

    SECTION("edge list (undirected)") {
        std::vector<Edge> edge_list = {
            {0, 1, 10}, {0, 2, 20}, {0, 4, 50},
            {1, 3, 30}, {2, 3, 40}, {2, 4, 60}, {3, 4, 70}
        };

        Converter converter(edge_list, false);
        Graph_ expected = {
            {{1, 10}, {2, 20}, {4, 50}},
            {{0, 10}, {3, 30}},
            {{0, 20}, {3, 40}, {4, 60}},
            {{1, 30}, {2, 40}, {4, 70}},
            {{0, 50}, {2, 60}, {3, 70}}
        };

        REQUIRE(converter.graph == expected);
    }

    SECTION("edge list (directed)") {
        std::vector<Edge> edge_list = {
            {0, 1, 10}, {0, 2, 20}, {0, 4, 50},
            {1, 3, 30}, {2, 3, 40}, {2, 4, 60}, {3, 4, 70}
        };

        Converter converter(edge_list, true);
        Graph_ expected = {
            {{1, 10}, {2, 20}, {4, 50}},
            {{3, 30}},
            {{3, 40}, {4, 60}},
            {{4, 70}},
            {}
        };

        REQUIRE(converter.graph == expected);
    }

    SECTION("empty edge list") {
        std::vector<Edge> edge_list = {};
        Converter converter(edge_list, false);
        REQUIRE(converter.graph.empty());
    }

    SECTION("empty adjacency list") {
        Graph_ empty_list = {};
        Converter converter(empty_list);
        REQUIRE(converter.graph.empty());
    }

    SECTION("empty adjacency matrix") {
        int n = 5;
        long long** adjacency_matrix = new long long*[n];
        for (int i = 0; i < n; i++) {
            adjacency_matrix[i] = new long long[n]{};
        }

        Converter converter(adjacency_matrix, n);
        REQUIRE(converter.graph == Graph_(n));

        for (int i = 0; i < n; i++) {
            delete[] adjacency_matrix[i];
        }
        delete[] adjacency_matrix;
    }
}


TEST_CASE( "Correctness", "[Requirement 1]" ) {
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
        Converter edge_list_converter = Converter(edge_list, false);
        assert(edge_list_converter.graph.empty());
        std::pair<long long, std::vector<int>> expected = {-1,std::vector<int>()};

        // Edge list is empty, so inner assertion fails and crushes the program. Can't be "checked" normally.
        SECTION("Dijkstra for high density") {
            auto res1 = dijkstra_high_density(0,2,adjacency_list_converter);
            auto res2 = dijkstra_high_density(0,2,adjacency_matrix_converter);

            REQUIRE( res1 == expected );
            REQUIRE( res2 == expected );
        }

        SECTION("Dijkstra for low density") {
            auto res1 = dijkstra_low_density(0,2,adjacency_list_converter);
            auto res2 = dijkstra_low_density(0,2,adjacency_matrix_converter);

            REQUIRE( res1 == expected );
            REQUIRE( res2 == expected );
        }

        SECTION("Bellman-Ford") {
            auto res1 = bellman_for_two_vertices(0,2,adjacency_list_converter);
            auto res2 = bellman_for_two_vertices(0,2,adjacency_matrix_converter);

            REQUIRE( res1 == expected );
            REQUIRE( res2 == expected );
        }


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
                Edge e = {i, j, 1};
                edge_list.push_back(e);
            }
        }

        Converter adjacency_list_converter = Converter(adjacency_list);
        Converter adjacency_matrix_converter = Converter(adjacency_matrix, 5);
        Converter edge_list_converter = Converter(edge_list, false);
        std::pair<long long, std::vector<int>> expected = {1,{0,3}};

        SECTION("Dijkstra for high density") {
            test_shortest_path(adjacency_list_converter, adjacency_matrix_converter, edge_list_converter, 0, 3, expected, dijkstra_high_density);
        }
        SECTION("Dijkstra for low density") {
            test_shortest_path(adjacency_list_converter, adjacency_matrix_converter, edge_list_converter, 0, 3, expected, dijkstra_low_density);
        }
        SECTION("Bellman-Ford") {
            test_shortest_path(adjacency_list_converter, adjacency_matrix_converter, edge_list_converter, 0, 3, expected, bellman_for_two_vertices);
        }

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

        std::vector<Edge>edge_list = {{0,1,100},{0,4,10},{0,3,20},{1,2,7},{2,3,20},{3,4,15}};

        Converter adjacency_list_converter = Converter(adjacency_list);
        Converter adjacency_matrix_converter = Converter(adjacency_matrix, 5);
        Converter edge_list_converter = Converter(edge_list, false);

        std::pair<long long, std::vector<int>> expected = {47,{0,3,2,1}};

        SECTION("Dijkstra for high density") {
            test_shortest_path(adjacency_list_converter, adjacency_matrix_converter, edge_list_converter, 0, 1, expected, dijkstra_high_density);
        }
        SECTION("Dijkstra for low density") {
            test_shortest_path(adjacency_list_converter, adjacency_matrix_converter, edge_list_converter, 0, 1, expected, dijkstra_low_density);
        }
        SECTION("Bellman-Ford") {
            test_shortest_path(adjacency_list_converter, adjacency_matrix_converter, edge_list_converter, 0, 1, expected, bellman_for_two_vertices);
        }

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

        std::vector<Edge>edge_list = {{0,3,20},{0,4,10},{1,2,7},{3,4,15}};
        Converter adjacency_list_converter = Converter(adjacency_list);
        Converter adjacency_matrix_converter = Converter(adjacency_matrix, 5);
        Converter edge_list_converter = Converter(edge_list, false);
        std::pair<long long, std::vector<int>> expected = {-1,std::vector<int>()};

        SECTION("Dijkstra for high density") {
            test_shortest_path(adjacency_list_converter, adjacency_matrix_converter, edge_list_converter, 2, 4, expected, dijkstra_high_density);
        }
        SECTION("Dijkstra for low density") {
            test_shortest_path(adjacency_list_converter, adjacency_matrix_converter, edge_list_converter, 2, 4, expected, dijkstra_low_density);
        }
        SECTION("Bellman-Ford") {
            test_shortest_path(adjacency_list_converter, adjacency_matrix_converter, edge_list_converter, 2, 4, expected, bellman_for_two_vertices);
        }

        for (int i = 0; i < n; i++) {
            delete adjacency_matrix[i];
        }
        delete adjacency_matrix;
    }
       
}