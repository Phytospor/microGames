#include "Dice.hpp"
#include <cstdlib>
#include <ctime>
#include <vector>

Dice::Dice(float xPos, float yPos, float s) : 
    x(xPos), 
    y(yPos), 
    size(s) {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
        number = 1;
}


void Dice::roll() {
    number = std::rand() % 6 + 1;
}

void Dice::draw(sf::RenderWindow& window){
    // Draw dice body

    sf::RectangleShape dice(sf::Vector2f(size, size));
    dice.setFillColor(sf::Color::White);
    dice.setOutlineColor(sf::Color::Black);
    dice.setOutlineThickness(3.0f);
    dice.setPosition(sf::Vector2f(x, y));
    window.draw(dice);

    drawPips(window);
}

void Dice::drawPips(sf::RenderWindow& window){
    float radius = size / 10.f;
    float offset = size / 4.f;
    std::vector<sf::Vector2f> positions;
    switch (number) {
        case 1:
            positions.push_back({x + size/2, y + size/2});
            break;
        case 2:
            positions.push_back({x + offset, y + offset});
            positions.push_back({x + 3*offset, y + 3*offset});
            break;
        case 3:
            positions.push_back({x + offset, y + offset});
            positions.push_back({x + size/2, y + size/2});
            positions.push_back({x + 3*offset, y + 3*offset});
            break;
        case 4:
            positions.push_back({x + offset, y + offset});
            positions.push_back({x + 3*offset, y + 3*offset});
            positions.push_back({x + offset, y + 3*offset});
            positions.push_back({x + 3*offset, y + offset});
            break;
        case 5:
            positions.push_back({x + offset, y + offset});
            positions.push_back({x + 3*offset, y + 3*offset});
            positions.push_back({x + offset, y + 3*offset});
            positions.push_back({x + 3*offset, y + offset});
            positions.push_back({x + size/2, y + size/2});
            break;
        case 6:
            positions.push_back({x + offset, y + offset});
            positions.push_back({x + 3*offset, y + 3*offset});
            positions.push_back({x + offset, y + 3*offset});
            positions.push_back({x + 3*offset, y + offset});
            positions.push_back({x + 3*offset, y + size/2});
            positions.push_back({x + offset, y + size/2});
            break;
    }

    for (auto& pos : positions) {
        sf::CircleShape pip(radius);
        pip.setFillColor(sf::Color::Black);
        pip.setPosition(sf::Vector2f(pos.x - radius, pos.y - radius));
        window.draw(pip);
    }
}


