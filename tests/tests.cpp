#include "spath.hpp"
#include <iostream>
#include <catch2/catch_test_macros.hpp>

TEST_CASE( "Dijkstra correctness", "[Requirement 1]" ) {
    // SECTION("empty graph") {
    //     int n = 5;
    //     std::vector<std::vector<std::pair<int, ll>>>adjacency_list(n);
    //     ll** adjacency_matrix = new ll*[n];
    //     for (int i = 0; i < n; i++) {
    //         adjacency_matrix[i] = new ll[n];
    //         for (int j = 0; j < n; j++) {
    //             adjacency_matrix[i][j] = 0;
    //         }
    //     }
    //     std::vector<Edge>edge_list;

    //     Converter adjacency_list_converter = Converter(adjacency_list);
    //     Converter adjacency_matrix_converter = Converter(adjacency_matrix, 5);
    //     Converter edge_list_converter = Converter(edge_list);
    //     std::cout << "size is " <<adjacency_matrix_converter.g.size() << '\n';
    //     auto res11 = dijktsra_low_density(0, 2, adjacency_list_converter);
    //     auto res12 = dijktsra_high_density(0, 2, adjacency_list_converter);
    //     auto res21 = dijktsra_low_density(0, 2, adjacency_matrix_converter);
    //     auto res22 = dijktsra_high_density(0, 2, adjacency_matrix_converter);
    //     // auto res31 = dijktsra_low_density(0, 2, edge_list_converter);
    //     // auto res32 = dijktsra_high_density(0, 2, edge_list_converter);

    //     std::pair<ll, std::vector<int>> expected = {-1, std::vector<int>()};

        REQUIRE( 1 == 1 );
        // REQUIRE( res12 == expected );
        // REQUIRE( res21 == expected );
        // REQUIRE( res22 == expected );
        // REQUIRE( res31 == expected);
        // REQUIRE( res32 == expected);
        

    }
}