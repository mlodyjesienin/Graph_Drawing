#include "FindStartingPositions.h"
#include <cmath>

FindStartingPositions::FindStartingPositions(Graph *graphArg): graph(graphArg), temperature(INITIAL_TEMPERATURE),
                        alpha(ALPHA_COEFFICIENT), iteration(0), maxX(WINDOW_WIDTH-100), maxY(WINDOW_HEIGHT - 100){
    n = graph->n;
    for(auto vertex : graph->vertices){
        int id = vertex->id;
        for(auto neighborList : vertex->neighbors){
            int neighbor = neighborList.first->id;
            if(id < neighbor) edgeList.emplace_back(id,neighbor);
        }
    }
}

State FindStartingPositions::initialState() {
    State firstState;
    for (int i=0; i<n; i++) {
        auto a =100 + static_cast<float>(rand() % static_cast<int>(maxX -100));
        auto b = static_cast<float>(rand() % static_cast<int>(maxY - 100));
        firstState.emplace_back(a,b);

    }
    return firstState;
}


float FindStartingPositions::orient(Point a, Point b, Point c){
    return (b.first - a.first)*(c.second - b.second) - (b.second - a.second)*(c.first - b.first);
}

bool FindStartingPositions::isIntersection(Point a, Point b, Point c, Point d){
    return (orient(a,b,c) * orient(a,b,d) < 0 ) && (orient(c,d,a) * orient(c,d,b) <0);
}

bool FindStartingPositions::edgePairOk(int a, int b, int c, int d){
    if(a!=c && a!=d && b !=c && b!=d) return true;
    return false;
}
float FindStartingPositions::calculateCost(State &state) {
    float cost =0.0;
    for(auto edge1 : edgeList){
        int a = edge1.first;
        int b = edge1.second;
        for(auto edge2 : edgeList){
            int c = edge2.first;
            int d = edge2.second;
            if(!edgePairOk(a,b,c,d)) continue;
            if(isIntersection(state[a], state[b], state[c], state[d])) cost++;
        }
    }
    return cost;
}

bool FindStartingPositions::inBoundaries(float x, float y) {
    return x >=100 && x <= WINDOW_WIDTH - 100 && y >= 100 && y <= WINDOW_HEIGHT - 100;
}

float FindStartingPositions::updateCost(int v) {
    float cost = 0;
    auto vertex = graph->vertices[v];
    for( auto edgePair : vertex->neighbors) {
        auto v2 = edgePair.first;
        int b = v2->id;
        for (auto edge2 : edgeList) {
            int c = edge2.first;
            int d =edge2.second;
            if(!edgePairOk(v,b,c,d)) continue;
            if(isIntersection(neighborState[v], neighborState[b], neighborState[c], neighborState[d])) cost++;

        }
    }
    return cost;
}
float FindStartingPositions::pAccept() {
    return std::exp(-std::abs(neighborCost - currentCost) / temperature);
}

void FindStartingPositions::generateNeighbor() {
    int flag;
    float x1,y1,x2,y2;
    int vertex1,vertex2;
    neighborState = currentState;
    if(static_cast<float>(rand()) / RAND_MAX > 0.7){
        flag = 0;
        vertex1 = rand() % n;
        x1 =neighborState[vertex1].first;
        y1 = neighborState[vertex1].second;
        while(true){
            x1 += -100 + rand() % 200;
            y1 += -100 + rand() % 200;
            if(inBoundaries(x1,y1)) break;
            x1 =neighborState[vertex1].first;
            y1 = neighborState[vertex1].second;
        }
        neighborState[vertex1] = {x1,y1};
    }
    else{
        flag =1;
        while(true){
            vertex1 = rand() % n;
            vertex2 = rand() % n;
            if(vertex1!=vertex2) break;
        }
        x1 = neighborState[vertex1].first;
        y1 = neighborState[vertex1].second;
        x2 = neighborState[vertex2].first;
        y2 = neighborState[vertex2].second;
        neighborState[vertex1] = {x2,y2};
        neighborState[vertex2] = {x1,y1};
    }
    neighborCost = calculateCost(neighborState);
    if(neighborCost <= currentCost){
        currentCost = neighborCost;
        currentState[vertex1] = neighborState[vertex1];
        if(flag){
            currentState[vertex2] = neighborState[vertex2];
        }
    }
    else if(pAccept() > static_cast<float>(rand() / RAND_MAX)){
        currentCost = neighborCost;
        currentState[vertex1] = neighborState[vertex1];
        if(flag){
            currentState[vertex2] = neighborState[vertex2];
        }
    }
}


void FindStartingPositions::simulatedAnnealing() {
    srand(static_cast<unsigned int>(time(nullptr)));
    currentState = initialState();
    best = currentState;
    currentCost = calculateCost(currentState);
    minimalCost = currentCost;
    graph->firstCost = currentCost;
    for(auto vertex : graph->vertices){
        vertex->x = best[vertex->id].first;
        vertex->y = best[vertex->id].second;
    }
    for(; iteration < MAX_ITERATIONS; iteration++){
        generateNeighbor();
        if(currentCost <= minimalCost){
            minimalCost = currentCost;
            best = currentState;
        }
        temperature*=alpha;
    }

    graph->minimalCost = minimalCost;
    graph->finalPositions = best;
}