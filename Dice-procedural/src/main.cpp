#include <SFML/Graphics.hpp>
#include "Dice.hpp"


int main() {
    sf::RenderWindow window(sf::VideoMode({400,400}), "Dice Roller");
    Dice dice(150.f, 150.f, 100.f); // position x, y and size. I think of the upper right corner
    const sf::Font font("../assets/arial.ttf");

    sf::Text title(font, "Today is your lucky day :)");
    title.setCharacterSize(30);
    title.setStyle(sf::Text::Bold);
    title.setPosition({20.f,30.f});
    title.setFillColor(sf::Color(23, 65, 67));

    sf::Text instruct(font, "Press Spacebar to roll the dice");
    instruct.setCharacterSize(20);
    instruct.setStyle(sf::Text::Regular);
    instruct.setPosition({30.f,100.f});
    instruct.setFillColor(sf::Color(66, 122, 118));
    

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            if (auto* keyEvent = event->getIf<sf::Event::KeyPressed>()){
                if (event->getIf<sf::Event::KeyPressed>()->scancode == sf::Keyboard::Scancode::Space)
                    dice.roll();
                    instruct.setString("Roll again!");
                if (event->getIf<sf::Event::KeyPressed>()->scancode == sf::Keyboard::Scancode::Escape) 
                    window.close();
                }
        }
        window.clear(sf::Color(249, 180, 135));
        dice.draw(window);
        window.draw(title);
        window.draw(instruct);
        window.display();
    }
    return 0;
}
