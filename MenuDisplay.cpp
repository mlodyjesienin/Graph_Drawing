#include "MenuDisplay.h"
MenuDisplay::MenuDisplay()
        : window(sf::VideoMode(800, 600), "Menu Display"),
          inputBox(sf::Vector2f(400, 50)),
          buttonBox(sf::Vector2f(200, 50)) {

    // Load font
    if (!font.loadFromFile("../calibri-regular.ttf")) {
        std::cerr << "Error loading font\n";
    }

    // Setup input label
    inputLabel.setFont(font);
    inputLabel.setString("Input:");
    inputLabel.setCharacterSize(24);
    inputLabel.setFillColor(sf::Color::Black);
    inputLabel.setPosition(200, 160);

    // Setup input text
    inputText.setFont(font);
    inputText.setCharacterSize(24);
    inputText.setFillColor(sf::Color::Black);
    inputText.setPosition(210, 210);  // Slightly inside the input box

    // Setup input box
    inputBox.setFillColor(sf::Color::White);
    inputBox.setPosition(200, 200);
    inputBox.setOutlineColor(sf::Color::Black);
    inputBox.setOutlineThickness(2);

    // Setup button text
    buttonText.setFont(font);
    buttonText.setString("Create Graph");
    buttonText.setCharacterSize(24);
    buttonText.setFillColor(sf::Color::Black);

    // Center the button text
    sf::FloatRect textRect = buttonText.getLocalBounds();
    buttonText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    buttonText.setPosition(400, 375);  // Center position of the button

    // Setup button box
    buttonBox.setFillColor(sf::Color::Blue);
    buttonBox.setPosition(300, 350);
    buttonBox.setOutlineColor(sf::Color::Black);
    buttonBox.setOutlineThickness(2);
}

void MenuDisplay::run() {
    while (window.isOpen()) {
        processEvents();
        update();
        render();
    }
}

void MenuDisplay::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::TextEntered:
                handleInput(event);
                break;
            case sf::Event::KeyPressed:
                if (event.key.control && event.key.code == sf::Keyboard::V) {
                    userInput += sf::Clipboard::getString();
                } else if (event.key.control && event.key.code == sf::Keyboard::C) {
                    sf::Clipboard::setString(userInput);
                } else if (event.key.code == sf::Keyboard::Enter) {
                    handleButtonPress();
                }
                break;
            case sf::Event::MouseButtonPressed:
                if (buttonBox.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                    handleButtonPress();
                }
                break;
            default:
                break;
        }
    }
}

void MenuDisplay::update() {
    inputText.setString(userInput);
}

void MenuDisplay::render() {
    window.clear(sf::Color::White);
    window.draw(inputLabel);
    window.draw(inputBox);
    window.draw(inputText);
    window.draw(buttonBox);
    window.draw(buttonText);
    window.display();
}

void MenuDisplay::handleInput(const sf::Event& event) {
    if (event.text.unicode < 128) {
        char typedChar = static_cast<char>(event.text.unicode);
        if (typedChar == '\b' && !userInput.empty()) {
            userInput.pop_back();  // Handle backspace
        } else if (typedChar >= 32 && typedChar <= 126) {
            userInput += typedChar;  // Add characters to the input string
        }
    }
}

void MenuDisplay::handleButtonPress() {
    std::vector<std::vector<int>> adjacencyList = parseInput(userInput);
    Graph graph = Graph(adjacencyList);
    graph.setRandomWeights(0, 50);

    window.close();

    graph.drawGraph();
}

std::vector<std::vector<int>> MenuDisplay::parseInput(const std::string& input) {
    std::vector<std::vector<int>> adjacencyList;
    std::string cleanedInput;
    std::remove_copy_if(input.begin(), input.end(), std::back_inserter(cleanedInput), [](char c) { return std::isspace(c); });

    std::stringstream ss(cleanedInput);
    std::string token;
    while (std::getline(ss, token, '[')) {
        if (token.empty()) continue;
        token.pop_back(); // Remove trailing ']'
        std::vector<int> neighbors;
        std::stringstream neighborStream(token);
        std::string neighbor;
        while (std::getline(neighborStream, neighbor, ',')) {
            neighbors.push_back(std::stoi(neighbor));
        }
        adjacencyList.push_back(neighbors);
    }

    return adjacencyList;
}

void MenuDisplay::changeButtonColorTemporary(sf::Color color, sf::Time duration) {
    buttonBox.setFillColor(color);
    render();
    std::this_thread::sleep_for(std::chrono::milliseconds(duration.asMilliseconds()));
    buttonBox.setFillColor(sf::Color::Blue);
}