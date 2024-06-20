#include <SFML/Graphics.hpp>
#include <vector>
#include "graph.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

int main(){

    std::vector<std::vector<int>> adjacencyList5 = {
            {1, 2},       // Neighbors of node 0
            {0, 3, 4},    // Neighbors of node 1
            {0, 3},       // Neighbors of node 2
            {1, 2, 4},    // Neighbors of node 3
            {1, 3}        // Neighbors of node 4
    };
    std::vector<std::vector<int>> adjacencyList11 = {
            {1, 2, 3},             // Neighbors of node 0
            {0, 4, 5},             // Neighbors of node 1
            {0, 6, 7},             // Neighbors of node 2
            {0, 8, 9},             // Neighbors of node 3
            {1, 10, 11},           // Neighbors of node 4
            {1},           // Neighbors of node 5
            {2},           // Neighbors of node 6
            {2},           // Neighbors of node 7
            {3},           // Neighbors of node 8
            {3},                   // Neighbors of node 9
            {4},                   // Neighbors of node 10
            {4},                   // Neighbors of node 11
            // Neighbors of node 14
                 // Neighbors of node 17

    };

    std::vector<std::vector<int>> adjacencyList20 = {
            {1, 2, 3},            // Neighbors of node 0
            {0, 4, 5},            // Neighbors of node 1
            {0, 6, 7},            // Neighbors of node 2
            {0, 8, 9},            // Neighbors of node 3
            {1, 10, 11},          // Neighbors of node 4
            {1, 12, 13},          // Neighbors of node 5
            {2, 14, 15},          // Neighbors of node 6
            {2, 16, 17},          // Neighbors of node 7
            {3, 18, 19},          // Neighbors of node 8
            {3, 15, 17},          // Neighbors of node 9
            {4, 18, 19},          // Neighbors of node 10
            {4, 12, 13},          // Neighbors of node 11
            {5, 11, 14},          // Neighbors of node 12
            {5, 11, 19},          // Neighbors of node 13
            {6, 12, 18},          // Neighbors of node 14
            {6, 9, 16},           // Neighbors of node 15
            {7, 15, 17},          // Neighbors of node 16
            {7, 9, 16},           // Neighbors of node 17
            {8, 10, 14},          // Neighbors of node 18
            {8, 10, 13}           // Neighbors of node 19
    };
    std::vector<std::vector<int>> adjacencyList50 = {
            {1, 2, 3},               // Neighbors of node 0
            {0, 4, 5},               // Neighbors of node 1
            {0, 6, 7},               // Neighbors of node 2
            {0, 8, 9},               // Neighbors of node 3
            {1, 10, 11},             // Neighbors of node 4
            {1, 12, 13},             // Neighbors of node 5
            {2, 14, 15},             // Neighbors of node 6
            {2, 16, 17},             // Neighbors of node 7
            {3, 18, 19},             // Neighbors of node 8
            {3, 20, 21},             // Neighbors of node 9
            {4, 22, 23},             // Neighbors of node 10
            {4, 24, 25},             // Neighbors of node 11
            {5, 26, 27},             // Neighbors of node 12
            {5, 28, 29},             // Neighbors of node 13
            {6, 30, 31},             // Neighbors of node 14
            {6, 32, 33},             // Neighbors of node 15
            {7, 34, 35},             // Neighbors of node 16
            {7, 36, 37},             // Neighbors of node 17
            {8, 38, 39},             // Neighbors of node 18
            {8, 40, 41},             // Neighbors of node 19
            {9, 42, 43},             // Neighbors of node 20
            {9, 44, 45},             // Neighbors of node 21
            {10, 46, 47},            // Neighbors of node 22
            {10, 48, 49},            // Neighbors of node 23
            {11, 46, 47},            // Neighbors of node 24
            {11, 48, 49},            // Neighbors of node 25
            {12, 42, 43},            // Neighbors of node 26
            {12, 44, 45},            // Neighbors of node 27
            {13, 38, 39},            // Neighbors of node 28
            {13, 40, 41},            // Neighbors of node 29
            {14, 34, 35},            // Neighbors of node 30
            {14, 36, 37},            // Neighbors of node 31
            {15, 30, 31},            // Neighbors of node 32
            {15, 32, 33},            // Neighbors of node 33
            {16, 26, 27},            // Neighbors of node 34
            {16, 28, 29},            // Neighbors of node 35
            {17, 18, 19},            // Neighbors of node 36
            {17, 20, 21},            // Neighbors of node 37
            {18, 22, 23},            // Neighbors of node 38
            {18, 24, 25},            // Neighbors of node 39
            {19, 42, 43},            // Neighbors of node 40
            {19, 44, 45},            // Neighbors of node 41
            {20, 30, 31},            // Neighbors of node 42
            {20, 32, 33},            // Neighbors of node 43
            {21, 34, 35},            // Neighbors of node 44
            {21, 36, 37},            // Neighbors of node 45
            {22, 38, 39},            // Neighbors of node 46
            {22, 40, 41},            // Neighbors of node 47
            {23, 42, 43},            // Neighbors of node 48
            {23, 44, 45}             // Neighbors of node 49
    };


    Graph graph = Graph(adjacencyList11);
    graph.setRandomWeights(0,50);

    srand(static_cast<unsigned int>(time(nullptr))); // Seed the random number generator


    graph.drawGraph();
}
