#include "GraphDisplay.h"
#include "graph.h"
#include <cmath>
#include <filesystem>
#include <csignal>
#include <numeric>

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
    if(graph->type==0) isWeightDisplayed = false;
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
void GraphDisplay::addButton(const sf::Vector2f& size, const sf::Vector2f& position, const std::function<void()>& function, const std::string& label) {
    sf::RectangleShape button;
    button.setSize(size);
    button.setFillColor(sf::Color(100, 100, 250));
    button.setPosition(position);
    buttons.push_back(button);
    buttonFunctions.push_back(function);

    sf::Text text;
    text.setFont(font);
    text.setString(label);
    text.setCharacterSize(16);
    text.setFillColor(sf::Color::White);
    text.setPosition(position.x + 10, position.y + 5); // Adjust position as needed
    buttonLabels.push_back(text);
}

void GraphDisplay::drawGraphSpecifics(){
    int n = graph->n;
    auto &vertices = graph->vertices;
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

void GraphDisplay::firstFrames(){
    sf::Text text;
    text.setFont(font);

    std::stringstream ss;
    ss << "First iteration number of intersections: " << graph->firstCost;
    text.setString(ss.str());
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    text.setPosition(window.getSize().x / 2.0f, 50.0f);  // 0.0f to position at the top
    text.setCharacterSize(25);
    text.setFillColor(sf::Color::Black);
    drawGraphSpecifics();
    window.clear(backgroundColor);
    window.draw(text);
    for (const auto& pair_edge : edgesDisplay) {
        const sf::RectangleShape& edge = pair_edge.second;
        window.draw(edge);
    }

    for (const auto& vertex : verticesDisplay) {
        window.draw(vertex);
    }
    window.display();
    sleep(4);
    verticesDisplay.clear();
    edgesDisplay.clear();
    for(auto& vertex : graph->vertices){
        vertex->x = graph->finalPositions[vertex->id].first;
        vertex->y = graph->finalPositions[vertex->id].second;
    }
    idDisplay.clear();
    weightsDisplay.clear();
    drawGraphSpecifics();
    std::stringstream secondtext;
    secondtext << "Last iteration number of intersections: " << graph->minimalCost;
    text.setString(secondtext.str());
    window.clear(backgroundColor);
    window.draw(text);
    for (const auto& pair_edge : edgesDisplay) {
        const sf::RectangleShape& edge = pair_edge.second;
        window.draw(edge);
    }

    for (const auto& vertex : verticesDisplay) {
        window.draw(vertex);
    }
    window.draw(text);
    window.display();

    sleep(3);
    window.clear(backgroundColor);
}
void GraphDisplay::initializeDisplay() {
    window.create(sf::VideoMode(windowWidth, windowHeight), "Graph Visualization: ",
                  sf::Style::Titlebar | sf::Style::Close);

    firstFrames();
    bar.setSize(sf::Vector2f(window.getSize().x, 50));
    bar.setFillColor(sf::Color(50, 50, 50));
    bar.setPosition(0, 0);

    // Initialize buttons
    addButton(sf::Vector2f(100, 30), sf::Vector2f(10, 10), [this] { showBFS(); },"BFS");
    addButton(sf::Vector2f(100, 30), sf::Vector2f(120, 10), [this] {showShortestPath(); }, "PATH");
    addButton(sf::Vector2f(100, 30), sf::Vector2f(230, 10), [this] {showKruskal();}, "MST");

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
                        for(int i=0; i< buttons.size(); i++){
                            if (buttons[i].getGlobalBounds().contains(mousePos)){
                                buttonFunctions[i]();
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

    window.draw(bar);

    for(const auto& button : buttons) {
        window.draw(button);
    }

    for(const auto& label: buttonLabels){
        window.draw(label);
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
void GraphDisplay::showBFS() {
    if(selected.size() != 1){
        std::cout << "nie!" << std::endl;
        return;
    }
    struct bfsData data = graph->BFS(selected[0]);
    auto maxDepthIter = std::max_element(data.distance.begin(), data.distance.end());
    int maxDepth;
    if (maxDepthIter != data.distance.end()) {
         maxDepth = *maxDepthIter;
    }
    int depth = 1;
    while(depth <= maxDepth){
        for(int i=0; i<graph->n; i++){
            if(data.distance[i]==depth){
                int j = data.parent[i];
                std::pair<int, int> edgeKey = (i < j) ? std::make_pair(i, j)
                                                                           : std::make_pair(j, i);
                auto edge = &edgesDisplay[edgeKey];
                edge->setFillColor(sf::Color::Green);
                verticesDisplay[i].setFillColor(sf::Color::Green);
            }
        }
        windowDraw();
        depth++;
        sleep(2);

    }
    sleep(4);
    resetAll();

}

void GraphDisplay::showShortestPath(){
    if(selected.size() != 2){
        std::cout << "nie!" << std::endl;
        return;
    }
    struct bfsData data = graph->BFS(selected[0]);
    int s = selected[0];
    int t = selected[1];
    std::vector<int> path;
    int tmp = t;
    path.push_back(tmp);
    while(data.parent[tmp]!=s){
        tmp = data.parent[tmp];
        path.push_back(tmp);
    }
    if(path[path.size()-1]!=s) path.push_back(s);

    std::reverse(path.begin(), path.end());
    int maxDepth = data.distance[t];
    int depth = 1;
    while(depth <= maxDepth){
        for(int i=0; i<graph->n; i++){
            if(data.distance[i]==depth){
                int j = data.parent[i];
                std::pair<int, int> edgeKey = (i < j) ? std::make_pair(i, j)
                                                      : std::make_pair(j, i);
                auto edge = &edgesDisplay[edgeKey];
                edge->setFillColor(sf::Color::Green);
                verticesDisplay[i].setFillColor(sf::Color::Green);
                edgeKey = (path[depth-1] < path[depth]) ? std::make_pair(path[depth-1], path[depth])
                                  : std::make_pair(path[depth], path[depth-1]);
                auto edge2 = &edgesDisplay[edgeKey];
                edge2->setFillColor(sf::Color::Red);
                verticesDisplay[path[depth]].setFillColor(sf::Color::Red);
            }
        }
        windowDraw();
        depth++;
        sleep(2);

    }
    sleep(4);
    resetAll();
}

using Edge = std::tuple<int, int, float>;

void GraphDisplay::showKruskal() {
    std::vector<Edge> edges = graph->getEdgeList();
    // Sort edges by weight
    std::sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return std::get<2>(a) < std::get<2>(b);
    });

    int n = graph->n;

    std::vector<int> parent(n);
    std::iota(parent.begin(), parent.end(), 0); // Initialize each vertex as its own parent

    std::function<int(int)> find = [&](int u) {
        if (parent[u] != u) {
            parent[u] = find(parent[u]);
        }
        return parent[u];
    };

    auto unite = [&](int u, int v) {
        int root_u = find(u);
        int root_v = find(v);
        if (root_u != root_v) {
            parent[root_v] = root_u;
        }
    };

    std::vector<Edge> mst;
    for (const Edge& edge : edges) {
        int u = std::get<0>(edge);
        int v = std::get<1>(edge);
        std::pair<int, int> edgeKey = (u < v) ? std::make_pair(u, v)
                                              : std::make_pair(v, u);
        auto edgeDis = &edgesDisplay[edgeKey];
        edgeDis->setFillColor(sf::Color::Green);
        float weight = std::get<2>(edge);
        if (find(u) != find(v)) {
            unite(u, v);

            edgeDis->setFillColor(sf::Color::Red);
            verticesDisplay[u].setFillColor(sf::Color::Red);
            verticesDisplay[v].setFillColor(sf::Color::Red);
            windowDraw();
            sleep(1);
            mst.push_back(edge);
        }
    }
    sleep(4);
    resetAll();
}



void GraphDisplay::resetAll() {
    for(auto& vertex: verticesDisplay){
        vertex.setFillColor(VertexColor);
    }
    for(auto& edgePair:edgesDisplay){
        auto edge = &edgePair.second;
        edge->setFillColor(sf::Color::Black);
    }
}