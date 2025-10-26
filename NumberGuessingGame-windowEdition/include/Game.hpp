#ifndef GAME_HPP //could also use Pragma
#define GAME_HPP

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <string>


class Game {
    public:
        Game();                 // Constructor
        void run();             // Run the main loop

    private:
        sf::RenderWindow window;
        sf::Font font;
        sf::Text inputText;
        sf::Text messageText;
        sf::Text counterText;

        std::string userInput;
        std::string message;
        sf::Clock gameClock;
        sf::Text timerText;
        int target;
        int guessCount;

        bool gameOver = false;
        float finalTime = 0.f;

        void handleInput(sf::Event& event);
        void update();
        void render();
        void resetGame();

};

#endif
