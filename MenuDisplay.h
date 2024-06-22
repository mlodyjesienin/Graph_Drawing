#ifndef GRAPH_DRAWING_MENUDISPLAY_H
#define GRAPH_DRAWING_MENUDISPLAY_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>
#include "graph.h"
#include <SFML/Window/Clipboard.hpp>

class MenuDisplay {
public:
    MenuDisplay();
    void run();

private:
    void processEvents();
    void update();
    void render();
    void handleInput(const sf::Event& event);
    void handleButtonPress();
    void changeButtonColorTemporary(sf::Color color, sf::Time duration);
    std::vector<std::vector<int>> parseInput(const std::string& input);

    sf::RenderWindow window;
    sf::Font font;
    sf::Text inputText;
    sf::Text buttonText;
    sf::Text inputLabel;
    sf::RectangleShape inputBox;
    sf::RectangleShape buttonBox;
    std::string userInput;
};

#endif //GRAPH_DRAWING_MENUDISPLAY_H
