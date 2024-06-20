#include <queue>
#include "graph.h"
#include "GraphDisplay.h"
#include "FindStartingPositions.h"

Graph::Graph(std::vector<std::vector<int>> adjacency_list) {
    type = 0;
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
    FindStartingPositions positions = FindStartingPositions(this);
    positions.simulatedAnnealing();
    graphDisplay = new GraphDisplay(this);
    graphDisplay->draw();
}

struct bfsData Graph::BFS(int s) {
    struct bfsData data;
    auto visited = data.visited;
    auto d = data.distance;
    auto parent = data.parent;
    visited.assign(n, false);
    d.assign(n, -1);
    parent.assign(n, -1);

    std::queue<int> Q;
    visited[s] = true;
    d[s] = 0;
    Q.push(s);

    while (!Q.empty()) {
        int u = Q.front();
        Q.pop();
        for (auto& neighbor : vertices[u]->neighbors) {
            int v = neighbor.first->id;
            if (!visited[v]) {
                visited[v] = true;
                parent[v] = u;
                d[v] = d[u] + 1;
                Q.push(v);
            }
        }
    }
    data.parent = parent;
    data.visited = visited;
    data.distance = d;
    return data;
}

std::vector<std::tuple<int, int, float>> Graph::getEdgeList() {
    std::vector<std::tuple<int, int, float>> edges;

    for (Vertex* vertex : vertices) {
        int u = vertex->id;

        for (auto& neighborPair : vertex->neighbors) {
            Vertex* neighbor = neighborPair.first;
            float weight = neighborPair.second;
            int v = neighbor->id;

            if (u < v) {
                edges.emplace_back(u, v, weight);
            }
        }
    }

    return edges;
}

void Graph::setRandomWeights(int i, int j) {
    type = 1;
    for(auto &vertex : vertices){
        for(auto& neighbor : vertex->neighbors){
            int nid = neighbor.first->id;
            neighbor.second = nid + vertex->id / 2;
        }
    }
}

