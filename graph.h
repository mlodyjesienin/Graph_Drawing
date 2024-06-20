#ifndef GRAPH_H
#define GRAPH_H

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
//#include "GraphDisplay.h

class GraphDisplay;

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
    std::vector<Vertex*> vertices;
    int n;
    GraphDisplay * graphDisplay;

public:
    ~Graph();

    explicit Graph(std::vector<std::vector<int>> adjacency_list);

    void print_graph();

    void drawGraph();
};

#endif // GRAPH_H
