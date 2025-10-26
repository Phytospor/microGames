#ifndef DICE_HPP
#define DICE_HPP
//or #pragma instead - but not portable
#

#include <SFML/Graphics.hpp>

class Dice{
public:
    Dice(float x, float y, float size);

    void roll();    //randomly choose a number
    void draw(sf::RenderWindow& window); // draw dice on window

private:
    int number; // Current dice number
    float x, y; // Position
    float size; //size of dice
    void drawPips(sf::RenderWindow& window);
};



#endif // DICE_HPP