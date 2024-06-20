#ifndef GRAPH_DRAWING_FINDSTARTINGPOSITIONS_H
#define GRAPH_DRAWING_FINDSTARTINGPOSITIONS_H

#include "graph.h"
#include <cstdlib>
#include <ctime>
const double INITIAL_TEMPERATURE = 5000;
const double ALPHA_COEFFICIENT = 0.9997;
const int MAX_ITERATIONS = 10000;

using Point = std::pair<float,float>;
using State = std::vector<Point>;

class FindStartingPositions {
private:
    Graph * graph;
    std::vector<std::pair<int,int>> edgeList;
    double temperature;
    double alpha;
    State best;
    State currentState;
    State neighborState;
    float minimalCost;
    float currentCost;
    float neighborCost;
    int iteration;
    int maxX;
    int maxY;
    int n;

    State initialState();
    bool edgePairOk(int a, int b, int c, int d);
    static bool isIntersection(Point a, Point b, Point c, Point d);
    static float orient(Point a, Point b, Point c);
    float calculateCost(State &state);
    bool inBoundaries(float x, float y);
    float updateCost(int vertex);
    float  pAccept();
    void generateNeighbor();
public:
    explicit FindStartingPositions(Graph *g);
    void simulatedAnnealing();

};


#endif //GRAPH_DRAWING_FINDSTARTINGPOSITIONS_H
