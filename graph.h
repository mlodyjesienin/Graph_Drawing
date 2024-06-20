#ifndef GRAPH_H
#define GRAPH_H

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
//#include "GraphDisplay.h

class GraphDisplay;

struct bfsData{
    std::vector<bool> visited;
    std::vector<int> distance;
    std::vector<int> parent;
};

class Vertex {
public:
    int id;
    std::vector<std::pair<Vertex*, float>> neighbors; // Pair of neighbor and weight
    float x;
    float y;
    Vertex(int id) : id(id) {}

    void addNeighbor(Vertex* neighbor, float weight) {
        neighbors.emplace_back(neighbor, weight);
    }
};

class Graph {
public:
    int type;
    std::vector<Vertex*> vertices;
    int n;
    GraphDisplay * graphDisplay;

public:
    ~Graph();

    explicit Graph(std::vector<std::vector<int>> adjacency_list);

    void print_graph();

    void drawGraph();

    struct bfsData BFS(int s);

    std::vector<std::tuple<int, int, float>> getEdgeList();
    void setRandomWeights(int i,int j);
};

#endif // GRAPH_H
