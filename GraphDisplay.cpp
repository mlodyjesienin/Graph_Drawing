#include "GraphDisplay.h"
#include "graph.h"
#include <cmath>
#include <filesystem>


GraphDisplay::GraphDisplay(Graph *graphArg) : windowWidth(WINDOW_WIDTH), windowHeight(WINDOW_HEIGHT), graph(graphArg), draggingIndex(-1),
            weightFontSize(18){
    vertexRadius = windowHeight*windowWidth/(2000*graph->n);
    edgeThickness = vertexRadius/5;
    idFontSize = int(vertexRadius);
    weightFontSize = idFontSize *3/5;
    if(idFontSize<12) isIdDisplayed = false;
    else isIdDisplayed = true;
    if(weightFontSize<10) isWeightDisplayed = false;
    else isWeightDisplayed = true;
    if (!font.loadFromFile("../calibri-regular.ttf")) {
        std::cerr << "Error loading font from calibri-regular.ttf\n";
        exit(-1);
    }
}

GraphDisplay::~GraphDisplay() {
    window.close();
    verticesDisplay.clear();
    edgesDisplay.clear();
}

void GraphDisplay::setUpWeight(sf::Text* weight, const sf::Vector2f& startPoint, const sf::Vector2f& endPoint){
    weight->setFont(font);
    weight->setCharacterSize(weightFontSize);
    weight->setFillColor(sf::Color::Black);
    weight->setOutlineColor(sf::Color::White);
    weight->setOutlineThickness(weightFontSize/10);
    sf::Vector2f midpoint((startPoint.x + endPoint.x) / 2, (startPoint.y + endPoint.y) / 2);
    weight->setPosition(midpoint);
}
void GraphDisplay::calculateEdgePosition(sf::RectangleShape* edge, const sf::Vector2f& startPoint, const sf::Vector2f& endPoint, float distance) const {

    float angle = std::atan2(endPoint.y - startPoint.y, endPoint.x - startPoint.x) * 180 / M_PI;

    edge->setSize(sf::Vector2f(distance, edgeThickness)); // Adjust thickness as needed
    edge->setOrigin(0.0f, edgeThickness / 2.0f); // Center origin
    edge->setPosition(startPoint);
    edge->setRotation(angle);
    edge->setFillColor(sf::Color::Black);


}
void GraphDisplay::setUpVertices(int i){
    auto &vertices = graph->vertices;
    verticesDisplay.emplace_back();
    verticesDisplay.back().setRadius(vertexRadius);
    verticesDisplay.back().setPosition(vertices[i]->x - vertexRadius, vertices[i]->y - vertexRadius);
    verticesDisplay.back().setFillColor(VertexColor);
    verticesDisplay.back().setOutlineColor(sf::Color::Black);
    verticesDisplay.back().setOutlineThickness(vertexRadius/10);
}

void GraphDisplay::setUpID(int i){
    auto &vertices = graph->vertices;
    idDisplay.emplace_back();
    idDisplay.back().setFont(font);
    idDisplay.back().setString(std::to_string(vertices[i]->id));
    idDisplay.back().setCharacterSize(idFontSize); // Adjust size as needed
    idDisplay.back().setFillColor(sf::Color::Black);
    idDisplay.back().setOutlineColor(veryLightGray);
    idDisplay.back().setOutlineThickness(vertexRadius/40);
    sf::FloatRect textRect = idDisplay.back().getLocalBounds();
    idDisplay.back().setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    idDisplay.back().setPosition(vertices[i]->x, vertices[i]->y);
}
void GraphDisplay::initializeDisplay() {

    int n = graph->n;
    auto &vertices = graph->vertices;
    window.create(sf::VideoMode(windowWidth, windowHeight), "Graph Visualization: ",
                  sf::Style::Titlebar | sf::Style::Close);

    //initialize vertices and description of vertices
    for (int i = 0; i < n; ++i) {
        setUpVertices(i);
        if(isIdDisplayed) setUpID(i);
    }

    //initialize edges and weights
    for (auto vertex : graph->vertices) {
        for (auto neighbor : vertex->neighbors) {
            int vid = vertex->id;
            int nid = neighbor.first->id;
            if(nid<vid) continue;

            sf::Vector2f startPoint(vertex->x, vertex->y);
            sf::Vector2f endPoint(neighbor.first->x, neighbor.first->y);
            float distance = std::sqrt(std::pow(endPoint.x - startPoint.x, 2) + std::pow(endPoint.y - startPoint.y, 2));
            sf::RectangleShape edge(sf::Vector2f(distance, edgeThickness));
            calculateEdgePosition(&edge,startPoint,endPoint,distance);
            edgesDisplay[{vertex->id, neighbor.first->id}] = edge;

            if(isWeightDisplayed){
                sf::Text weight;
                std::stringstream ss;
                ss << std::fixed << std::setprecision(2) << neighbor.second;
                weight.setString(ss.str());
                setUpWeight(&weight,startPoint,endPoint);
                weightsDisplay[{vertex->id, neighbor.first->id}] = weight;
            }
        }
    }
}

void GraphDisplay::draw() {
    int n = graph->n;
    auto& vertices = graph->vertices;
    sf::Vector2f offset;
    initializeDisplay();
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;

                case sf::Event::MouseButtonPressed:
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        bool flag = true;
                        sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                        for (int i = 0; i < n; ++i) {
                            if (verticesDisplay[i].getGlobalBounds().contains(mousePos)) {
                                draggingIndex = i;
                                addSelection(i);
                                offset = verticesDisplay[i].getPosition() - mousePos;
                                flag = false;
                                break;
                            }
                        }
                        if (flag) {
                            clearSelection();
                        }
                    }
                    break;

                case sf::Event::MouseButtonReleased:
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (draggingIndex != -1) {
                            auto new_pos = verticesDisplay[draggingIndex].getPosition();
                            vertices[draggingIndex]->x = new_pos.x;
                            vertices[draggingIndex]->y = new_pos.y;
                            draggingIndex = -1;
                        }
                    }
                    break;

                case sf::Event::MouseMoved:
                    if (draggingIndex != -1) {
                        dragVertexHandler(offset);
                    }
                    break;

                default:
                    break;
            }
        }

        windowDraw();
    }
}

void GraphDisplay::windowDraw(){
    window.clear(backgroundColor);

    for (const auto& pair_edge : edgesDisplay) {
        const sf::RectangleShape& edge = pair_edge.second;
        window.draw(edge);
    }
    if(isWeightDisplayed){
        for (const auto& pair_edge : weightsDisplay) {
            const sf::Text& weight = pair_edge.second;
            window.draw(weight);
        }
    }


    for (const auto& vertex : verticesDisplay) {
        window.draw(vertex);
    }

    if(isIdDisplayed){
        for (const auto& vertexID : idDisplay){
            window.draw(vertexID);
        }
    }


    window.display();
}
void GraphDisplay::clearSelection(){
    for(int i : selected){
        verticesDisplay[i].setFillColor(VertexColor);
    }
    selected.clear();
}

void GraphDisplay::addSelection(int i) {
    if(! sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) &&  !sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)){
        clearSelection();
    }
    selected.push_back(i);
    verticesDisplay[i].setFillColor(SelectedVertexColor);
}
void GraphDisplay::dragVertexHandler(sf::Vector2f offset){
    auto& vertices = graph->vertices;
    sf::Vector2f mousePos(event.mouseMove.x, event.mouseMove.y);
    verticesDisplay[draggingIndex].setPosition(mousePos + offset);
    sf::Vector2f newVertexPosition = verticesDisplay[draggingIndex].getPosition();
    idDisplay[draggingIndex].setPosition(newVertexPosition + sf::Vector2f(vertexRadius, vertexRadius));

    // Update positions of all edges connected to the dragging vertex
    for (auto neighbor : vertices[draggingIndex]->neighbors) {
        int neighborId = neighbor.first->id;

        // Determine the pair of vertices for the edge
        std::pair<int, int> edgeKey = (draggingIndex < neighborId) ? std::make_pair(draggingIndex, neighborId)
                                                                   : std::make_pair(neighborId, draggingIndex);

        // Retrieve the existing edge from the map
        sf::RectangleShape& edge = edgesDisplay[edgeKey];

        // Calculate new positions based on vertex centers
        sf::Vector2f startPoint(verticesDisplay[draggingIndex].getPosition().x + verticesDisplay[draggingIndex].getRadius(),
                                verticesDisplay[draggingIndex].getPosition().y + verticesDisplay[draggingIndex].getRadius());

        sf::Vector2f endPoint(verticesDisplay[neighborId].getPosition().x + verticesDisplay[neighborId].getRadius(),
                              verticesDisplay[neighborId].getPosition().y + verticesDisplay[neighborId].getRadius());

        float distance = std::sqrt(std::pow(endPoint.x - startPoint.x, 2) + std::pow(endPoint.y - startPoint.y, 2));
        calculateEdgePosition(&edge,startPoint,endPoint,distance);
        sf::Text& weightText = weightsDisplay[edgeKey];
        sf::Vector2f midpoint((startPoint.x + endPoint.x) / 2, (startPoint.y + endPoint.y) / 2);
        weightText.setPosition(midpoint);


    }
}