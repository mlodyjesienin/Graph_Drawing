#include <SFML/Graphics.hpp>
#include <vector>
#include "graph.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

int main(){
    /*std::vector<std::vector<int>> adjacencyList = {
            {1, 2, 3}, // Neighbors of node 0
            {0, 3},    // Neighbors of node 1
            {0, 3},    // Neighbors of node 2
            {0, 1, 2}  // Neighbors of node 3
    };*/

    /*int i=0;
    for(auto &vertex: graph.vertices){
        vertex->x = 100*(i%2+1);
        vertex->y = 100*(i/2 +1);
        i++;
    }*/
    std::vector<std::vector<int>> adjacencyList5 = {
            {1, 2},       // Neighbors of node 0
            {0, 3, 4},    // Neighbors of node 1
            {0, 3},       // Neighbors of node 2
            {1, 2, 4},    // Neighbors of node 3
            {1, 3}        // Neighbors of node 4
    };
    std::vector<std::vector<int>> adjacencyList20 = {
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

// Constructing a random graph with 100 vertices (randomized edges omitted for brevity)
    std::vector<std::vector<int>> adjacencyList100(50);
    for (int i = 0; i < 50; ++i) {
        for (int j = 0; j < 5; ++j) {
            int neighbor = rand() % 50;
            if (neighbor != i) {
                adjacencyList100[i].push_back(neighbor);
            }
        }
    }


    Graph graph = Graph(adjacencyList5);


    srand(static_cast<unsigned int>(time(nullptr))); // Seed the random number generator

    for (auto vertex : graph.vertices) {
        vertex->x = static_cast<float>(rand() % static_cast<int>(800));
        vertex->y = static_cast<float>(rand() % static_cast<int>(600));
    }


    graph.drawGraph();
}
