#include <iostream>
#include <cmath>    // for std::sqrt
#include <algorithm> // for std::swap
#include <memory>
#include <fstream>

#include <SFML/Graphics.hpp>


// create classes and functions in different header files or keep here in from of the main function

void reverseSpeed(float& x) 
{
    x = -x;
}

void circleBoundaries(sf::CircleShape &shape, float& shapeSpeedX, float& shapeSpeedY, float radius, int width, int height)
{
        if (shape.getPosition().x < 0)
        {
            reverseSpeed(shapeSpeedX);
            shape.setPosition({0, shape.getPosition().y});
        }

        else if (shape.getPosition().x + 2*radius > width)
        {
            reverseSpeed(shapeSpeedX);
            shape.setPosition({width - 2*radius, shape.getPosition().y});
        }

        if (shape.getPosition().y < 0)
        {
            reverseSpeed(shapeSpeedY);
            shape.setPosition({shape.getPosition().x, 0});
        }

        else if ((shape.getPosition().y + 2*radius > height))
        {
            reverseSpeed(shapeSpeedY);
            shape.setPosition({shape.getPosition().x, height - 2*radius});
        }
}

void collision(sf::CircleShape &shape1,sf::CircleShape &shape2, float &shape1Radius, float &shape2Radius, sf::Vector2f &shape1Velocity, sf::Vector2f &shape2Velocity)
{
    sf::Vector2f diff = shape2.getPosition() - shape1.getPosition();
        float distance = std::sqrt(diff.x*diff.x + diff.y*diff.y);
        if (distance < shape1Radius + shape2Radius)
        {
            // collision normal

            sf::Vector2f normal = diff / distance;

            // relative velocity along normal

            sf::Vector2f relativeVelocity = shape1Velocity - shape2Velocity;

            float velAlongNormal = relativeVelocity.x*normal.x + relativeVelocity.y*normal.y;
            float restitution = 1.0f; // 1.0 = perfectly elastic
            float impulse = -(1 + restitution) * velAlongNormal / 2; // equal mass


            // new velocities
            shape1Velocity += impulse * normal;
            shape2Velocity -= impulse * normal;

            // Optional overlap correction:
        }


}

// functions to declare: create circle function




int main(int argc, char const *argv[])
{   

    // create a new window of size w*h pixels
    // top-left of the window is (0,0) and bottom-right is (w,h)
    // you will have to read these from the config file -> figure out how to read in config files.
    // also, get config file from the video

    const int wWidth = 1280;
    const int wHeight = 720;

    sf::RenderWindow window(sf::VideoMode({wWidth, wHeight}), "Bouncy Shapes");
    window.setFramerateLimit(60); // limits frame rate to 60fps

    // create a clock used for its internal timing

    sf::Clock deltaClock; //

    // let's make a shape that we will draw to the screen
    float circleRadius = 50; // radius to draw the circle
    float circle2Radius = 25;
    int circleSegments = 32; // number of segments (polygons for approximation) to draw the circle with
    float circleSpeedX = 4.0f; // we will us this to move the circle later
    float circleSpeedY = 2.0f; // both must be read from input file
    float circle2SpeedX = 7.0f; // we will us this to move the circle later
    float circle2SpeedY = 3.0f; // both must be read from input file
    bool drawCircle = true; // whether or not to draw the circle
    bool drawText = true; // whether or not to draw the text

    sf::Vector2f circleSpeed({circleSpeedX,circleSpeedY});
    sf::Vector2f circle2Speed({circle2SpeedX,circle2SpeedY});
    
    // create the sfml circle shape based on our parameters
    sf::CircleShape circle(circleRadius, circleSegments);
    circle.setPosition({circleRadius, circleRadius}); // sets the top left position of the circle

    sf::CircleShape circle2(circleRadius, 32);
    circle2.setPosition({500.0f, 500.0f}); // sets the top left position of the circle

    // let's load a font so we can display some text

    sf::Font font;
    if (!font.openFromFile("../assets/BrownieStencil-8O8MJ.ttf")) 
    {
        throw std::runtime_error("Failed to load font!");
    };

    // set up the text object that will be drawn to the screen
    sf::Text myText(font, "I am here 2", 24);
    myText.setFillColor(sf::Color::White); // optional
    myText.setPosition({100, 100});   // optional

    // position the top-left corner of the text so that the text aligns on the bottom
    // text character size is in pixels, so move the text up from the bottom by its height
    myText.setPosition({10, wHeight - (float)myText.getCharacterSize()});

    // set up a character array to set the text
    char displayString[255] = "Sample Text";

    // main loop - continues for each frame while window is open
    while (window.isOpen())
    {
        // event handling
        while (auto eventOpt = window.pollEvent()) 
        {
            if (!eventOpt.has_value()) continue;
            auto& event = *eventOpt;

            // this event triggers when the window is closed
            if (event.is<sf::Event::Closed>())
            {
                window.close();
            }

            // this event is triggered when a key is pressed
            if (auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
                if (keyEvent->scancode == sf::Keyboard::Scancode::Escape)
                    window.close();
            }
        }

        // set the circle properties, because they may have been updated with the ui
        circle.setFillColor(sf::Color{55, 120, 200});
        circle.setPointCount(circleSegments);
        circle.setRadius(circleRadius);

        circle2.setFillColor(sf::Color{55, 120, 200});
        circle2.setPointCount(circleSegments);
        circle2.setRadius(circle2Radius);
        
        // basic animation move thet each frame if it's still in frame

        circle.setPosition({circle.getPosition().x + circleSpeed.x, circle.getPosition().y + circleSpeed.y});
        circle2.setPosition({circle2.getPosition().x + circle2Speed.x, circle2.getPosition().y + circle2Speed.y});

        // reverse speed if touching the sides
        circleBoundaries(circle, circleSpeed.x, circleSpeed.y, circleRadius, wWidth, wHeight);
        circleBoundaries(circle2, circle2Speed.x, circle2Speed.y, circle2Radius, wWidth, wHeight);

        // collision detection
    
        collision(circle, circle2, circleRadius, circle2Radius, circleSpeed, circle2Speed);


        
        // basic rendering function calls

        window.clear(); // clear the window of anything previously drawn
        if (drawCircle)
        {
            window.draw(circle);
            window.draw(circle2);
        }

        if (drawText) // draw the text if the boolean is true
        {
            window.draw(myText);
        }
        window.display(); // call the window display function
    }

    return 0;
}

