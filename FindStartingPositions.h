#ifndef GRAPH_DRAWING_FINDSTARTINGPOSITIONS_H
#define GRAPH_DRAWING_FINDSTARTINGPOSITIONS_H

#include "graph.h"
const double INITIAL_TEMPERATURE = 1000;
const double ALPHA_COEFFICIENT = 0.003;
const int MAX_ITERATIONS = 10000;

class FindStartingPositions {
private:
    Graph * graph;
    double temperature;
    double alpha;
    std::vector<std::pair<float,float>> best;
    std::vector<std::pair<float,float>> currentState;
    std::vector<std::pair<float,float>> neighborState;
    float minimalCost;
    float currentCost;
    float neighborCost;
    int iteration;

    float calculateEnergy(const std::vector<std::pair<float,float>> *state);
    float  pAccept(const float curr_cost, const float nghb_cost,  const double temperature);
    void generateNeighbor();
    void simulatedAnnealing();
public:
    FindStartingPositions(Graph g);
};


#endif //GRAPH_DRAWING_FINDSTARTINGPOSITIONS_H
