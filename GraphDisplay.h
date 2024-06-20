
#ifndef GRAPH_DRAWING_GRAPHDISPLAY_H
#define GRAPH_DRAWING_GRAPHDISPLAY_H

#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "graph.h"
#include <unordered_map>

const sf::Color VertexColor(65, 94, 206);
const sf::Color SelectedVertexColor(181, 55, 55);
const sf::Color veryLightGray(210, 210, 210);
const sf::Color backgroundColor = sf::Color::White;


class Graph;

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1,T2> &p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);

        return h1 ^ h2;
    }
};

class GraphDisplay {
private:
    sf::RenderWindow window;
    const int windowWidth;
    const int windowHeight;
    std::vector<sf::CircleShape> verticesDisplay;
    std::vector<sf::Text> idDisplay;
    std::unordered_map<std::pair<int, int>, sf::RectangleShape, pair_hash> edgesDisplay;
    std::unordered_map<std::pair<int, int>, sf::Text, pair_hash> weightsDisplay;
    sf::Font font;
    int idFontSize;
    int weightFontSize;
    sf::Event event;
    std::vector<int> selected;
    int draggingIndex;
    float vertexRadius;
    float edgeThickness;
    bool isWeightDisplayed;
    bool isIdDisplayed;
    Graph * graph;

    void setUpWeight(sf::Text *weight, const sf::Vector2f& startPoint, const sf::Vector2f& endPoint);
    void calculateEdgePosition(sf::RectangleShape* line, const sf::Vector2f& startPoint, const sf::Vector2f& endPoint, float distance) const;
    void initializeDisplay();
    void dragVertexHandler(sf::Vector2f offset);
    void clearSelection();
    void addSelection(int i);
    void setUpID(int i);
    void setUpVertices(int i);
    void windowDraw();


public:
    explicit GraphDisplay(Graph *graph);

    ~GraphDisplay();

    void draw();
};


#endif //GRAPH_DRAWING_GRAPHDISPLAY_H
