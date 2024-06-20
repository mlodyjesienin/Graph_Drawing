#include "graph.h"
#include "GraphDisplay.h"

Graph::Graph(std::vector<std::vector<int>> adjacency_list) {
    n = adjacency_list.size();
    for(int i=0; i <n ;i++){
        auto vertex = new Vertex(i);
        vertices.push_back(vertex);
    }
    for (int i = 0; i < n; ++i) {
        for (int neighbor : adjacency_list[i]) {
            vertices[i]->addNeighbor(vertices[neighbor],1);
        }
    }
}

Graph::~Graph() {
    for(auto vertex : vertices) delete vertex;
}

void Graph::print_graph() {
    for(auto vertex : vertices){
        std::cout << vertex->id << ": [ ";
        for(auto neighbor: vertex->neighbors){
            std::cout << neighbor.first->id << " ";
        }
        std::cout << "]" << std::endl;
    }
}

void Graph::drawGraph() {
    graphDisplay = new GraphDisplay(this);
    graphDisplay->draw();
}

