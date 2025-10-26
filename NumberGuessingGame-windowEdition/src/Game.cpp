#include "../include/Game.hpp"
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <iomanip>

using namespace std;

Game::Game() 
    :   font(),
        window(sf::VideoMode({800, 600}), "Number Guessing Game"), 
        userInput(""), 
        message("Guess a number between 1 and 100: "),
        guessCount(0),// initialize counter

        timerText(font,"", 30),
        counterText(font,"", 30),
        inputText(font, "", 30),
        messageText(font, "Guess a number between 1 and 100: ",40) {
        
            if (!font.openFromFile("../assets/Arial.ttf")) {
                throw std::runtime_error("Failed to load font!");
            }

            std::srand(static_cast<unsigned>(std::time(nullptr)));
            target = std::rand() % 100 + 1;

            gameClock.restart();

            timerText.setFillColor(sf::Color::Black);
            timerText.setPosition(sf::Vector2f{550.f,50.f});

            messageText.setFillColor(sf::Color(117, 138, 147, 255));
            messageText.setPosition(sf::Vector2f{100.f, 300.f});

            inputText.setFillColor(sf::Color(198, 110, 82, 155));
            inputText.setPosition(sf::Vector2f{300.f, 350.f});

            counterText.setFillColor(sf::Color::Black);
            counterText.setPosition(sf::Vector2f(300.f,500.f));
        }

void Game::handleInput(sf::Event& event) {

    if (event.is<sf::Event::TextEntered>()){
        auto* textEvent = event.getIf<sf::Event::TextEntered>();
        char typed = static_cast<char>(textEvent->unicode);

            if (typed >= '0' && typed <= '9') {
                userInput += typed;
                inputText.setString(userInput);

            } else if (typed == 8 && !userInput.empty()) { // Backspace
                userInput.pop_back();
                inputText.setString(userInput);

            } else if (typed == 'r' || typed == 'R') {
                resetGame();
            }
             else if (typed == '\r' || typed == '\n') { // Enter key - for some reason the sf::Keyboard::Scancode::Enter does not work 
                if (!userInput.empty()) {
                    
                    message = "Thank you for your guess";
                    messageText.setString(message);
                    guessCount ++; //increment counter
                    int guess = std::stoi(userInput);
                    if (guess < target) {
                        message = "Too low!\nTry again!";
                        counterText.setString("Guesses: " + std::to_string(guessCount));
                    }
                    else if (guess > target) {
                        message = "Too high!\nTry again!";
                        counterText.setString("Guesses: " + std::to_string(guessCount));
                    }

                    else if (guess == target) {
                        message = "Correct! Well Done!\nPress R to play again";
                        finalTime = gameClock.getElapsedTime().asSeconds();
                        counterText.setString("Final Guesses: " + std::to_string(guessCount));
                        gameOver = true;
                    }

                    messageText.setString(message);
                    

                    userInput.clear();
                    inputText.setString("");
                }
            }

        
            if (event.is<sf::Event::KeyPressed>()) {
                auto& keyEvent = *event.getIf<sf::Event::KeyPressed>();

            if (keyEvent.scancode == sf::Keyboard::Scancode::R) {
                resetGame();
            }
        }

    }
}

void Game::resetGame() {
    guessCount = 0;
    target = std::rand() % 100 + 1;
    message = "New Game: Guess a number between 1 and 100";
    messageText.setString(message);
    counterText.setString("Guesses: 0");
    gameClock.restart();
    gameOver = false;
    finalTime = 0.f;
}

void Game::update() {

    if (!gameOver) {
        float elapsed = gameClock.getElapsedTime().asSeconds();
        int minutes = static_cast<int>(elapsed) / 60;
        int seconds = static_cast<int>(elapsed) % 60;
        timerText.setString("Time: " + std::to_string(minutes) + ":" +
                            (seconds < 10 ? "0" : "") + std::to_string(seconds));
    } else {
        int minutes = static_cast<int>(finalTime) / 60;
        int seconds = static_cast<int>(finalTime) % 60;
        std::ostringstream ss;
        ss << "Final Time: "
            << std::setw(2) << std::setfill('0') << minutes
            << ":" << std::setw(2) << std::setfill('0') << seconds;

        timerText.setString(ss.str());
    }
}

void Game::render() {
    window.clear(sf::Color(236, 213, 188,125));
    window.draw(messageText);
    window.draw(inputText);
    window.draw(counterText);
    window.draw(timerText);
    window.display();
}



void Game::run() {
    while (window.isOpen()) {
        while (auto eventOpt = window.pollEvent()) {
            window.setVerticalSyncEnabled(true); //sets frame rate to the monitor frame rate.
            if (!eventOpt.has_value()) continue;
            auto& event = *eventOpt;

            // Close window
            if (event.is<sf::Event::Closed>()) {
                window.close();
            }
            // Optional: handle escape key
            if (auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
                if (keyEvent->scancode == sf::Keyboard::Scancode::Escape)
                    window.close();
            }

            // Handle text input
            handleInput(event);
        }

        update();
        render();
    }
}
