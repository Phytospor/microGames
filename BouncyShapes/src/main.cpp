#include <iostream>
#include <cmath>    // for std::sqrt
#include <algorithm> // for std::swap
#include <memory>
#include <fstream>
#include <filesystem>

#include <SFML/Graphics.hpp>
#include <imgui.h>
#include "../libraries/ImGui-SFML/imgui-SFML.h"

// create classes and functions in different header files or keep here in from of the main function

void reverseSpeed(float& x) 
{
    x = -x;
}

void circleBoundaries(sf::CircleShape 
    &shape, float& shapeSpeedX, float& shapeSpeedY, float radius, int width, int height)
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

// collision detection 
void collision(
    sf::CircleShape &shape1,
    sf::CircleShape &shape2, 
    float &shape1Radius, 
    float &shape2Radius, 
    sf::Vector2f &shape1Velocity, 
    sf::Vector2f &shape2Velocity,
    float friction)
{
    sf::Vector2f center1 = shape1.getPosition() + sf::Vector2f(shape1Radius, shape1Radius);
    sf::Vector2f center2 = shape2.getPosition() + sf::Vector2f(shape2Radius, shape2Radius);

    sf::Vector2f diff = center2 - center1;
    float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
    float minDistance = shape1Radius + shape2Radius;

    if (distance < minDistance) { // collision normal 
        sf::Vector2f normal = diff / distance; // relative velocity along normal 
        sf::Vector2f relativeVelocity = shape1Velocity - shape2Velocity; 
        float velAlongNormal = relativeVelocity.x*normal.x + relativeVelocity.y*normal.y;
        float restitution = 1.f; // 1.0 = perfectly elastic 
        float impulse = -(1 + restitution) * velAlongNormal / 2; // equal mass 

        // new velocities including friction control

        shape1Velocity += impulse * normal; 
        shape2Velocity -= impulse * normal; 
        
        // Optional overlap correction: 

        // --- overlap correction ---
        float overlap = minDistance - distance;
        sf::Vector2f correction = normal * (overlap * 0.5f); // push each shape half the overlap
        shape1.setPosition(shape1.getPosition() - correction);
        shape2.setPosition(shape2.getPosition() + correction);

        }
 }


// functions to declare: create circle function: one function to create the circle, one function to render the circle
sf::CircleShape createCircle (float circleRadius, int circleSegments, sf::Vector2f vectorPosition)
{
    
    // create the sfml circle shape based on our parameters
    sf::CircleShape circle(circleRadius, circleSegments);
    circle.setPosition({vectorPosition.x, vectorPosition.y}); // sets the top left position of the circle
    return circle;

}

struct ColorRGB {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};


ColorRGB hexToRGB(const std::string& hex) {
    std::string h = hex;

    // Support optional '#'
    if (h[0] == '#')
        h = h.substr(1);

    if (h.size() != 6)
        throw std::invalid_argument("Hex color must be 6 characters long");

    int value = std::stoi(h, nullptr, 16);

    ColorRGB c;
    c.r = (value >> 16) & 0xFF; // extract red
    c.g = (value >> 8) & 0xFF;  // extract green
    c.b = value & 0xFF;         // extract blue

    return c;
}

// Define a structure to hold circle data
struct CircleData {
    sf::CircleShape shape;
    sf::Vector2f velocity;
    float radius;
};


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

    // initialise IMGUI
    if (!ImGui::SFML::Init(window))
    {
        throw std::runtime_error("ImGui-SFML initialization failed");
    }
        // create a clock used for its internal timing
    sf::Clock deltaClock; //

    // scale ImGui UI by a given factor, does not affect text size

    ImGui::GetStyle().ScaleAllSizes(1.0f);

    // the imgui colour wheel requires floats from 0-1 instead of inits
    ColorRGB col;
    col = hexToRGB("6FA4AF");
    float c[3] = {col.r/255.f,col.g/255.f,col.b/255.f};

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

    sf::CircleShape circle2(circle2Radius, 32);
    circle2.setPosition({500.0f, 500.0f}); // sets the top left position of the circle


    float circle3Radius = 75; // radius to draw the circle
    float circle3SpeedX = 7.0f; // we will us this to move the circle later
    float circle3SpeedY = -3.0f;

    sf::Vector2f circle3Speed({circle3SpeedX,circle3SpeedY});
    sf::Vector2f circle3Position({200,200});
    sf::CircleShape circle3 = createCircle(circle3Radius, circleSegments, circle3Position);

    circle.setPointCount(circleSegments);
    circle.setRadius(circleRadius);

    circle2.setPointCount(circleSegments);
    circle2.setRadius(circle2Radius);

    circle3.setPointCount(circleSegments);
    circle3.setRadius(circle3Radius);

    // let's load a font so we can display some text
    std::filesystem::path exePath = std::filesystem::current_path(); // current working directory
    sf::Font font;
    if (!font.openFromFile((exePath / "assets/BrownieStencil-8O8MJ.ttf").string())) 
    {
        throw std::runtime_error("Failed to load font!");
    }

    // setup character array to set the text - this can be changed in the ImGui window
    char displayString[255] = "Sample Text";

    // set up the text object that will be drawn to the screen
    sf::Text myText(font, "I am here 2", 24);

    myText.setFillColor(sf::Color::White); // optional
    myText.setPosition({100, 100});   // optional

    sf::Rect<float> textBounds = myText.getLocalBounds();
    float padding = 10.f;
    myText.setPosition({
    wWidth - textBounds.size.x - padding,
    wHeight - textBounds.size.y - padding});

    // basic rendering function calls
    sf::Text myText2(font, displayString, 24);
    myText2.setPosition({10, wHeight - (float)myText.getCharacterSize()});

     // player

    float playerRadius = 50;
    int playerSegments = 32; // number of segments (polygons for approximation) to draw the circle with

    sf::CircleShape player(playerRadius, playerSegments);
    player.setPosition({wWidth/2, wHeight/2});  
    player.setPointCount(playerSegments);
    player.setRadius(playerRadius);

    sf::Vector2f playerSpeed({0.0f, 0.0f}); // initial velocity = 0
    float playerAcceleration = 100.f;  // pixels per second²
    float playerMaxSpeed = 500.f;      // pixels per second
    float playerFriction = 3.5f;      

    float ballFriction = 0.5f;

    // now I initialise the struct that contains all the circles
    std::vector<CircleData> dynamicCircles;

    // main loop - continues for each frame while window is open
    while (window.isOpen())
    {

        // set the circle properties, because they may have been updated with the ui

        sf::Color circleColor(
        static_cast<uint8_t>(c[0] * 255),
        static_cast<uint8_t>(c[1] * 255),
        static_cast<uint8_t>(c[2] * 255)
        );
        circle.setFillColor(circleColor);

        col = hexToRGB("B8C4A9");
        circle2.setFillColor(sf::Color{col.r, col.g, col.b});

        col = hexToRGB("D97D55");
        circle3.setFillColor(sf::Color{col.r, col.g, col.b});

        col = hexToRGB("334443");
        player.setFillColor(sf::Color{col.r, col.g, col.b});

        // event handling

        while (auto eventOpt = window.pollEvent()) 
        {
            if (!eventOpt.has_value()) continue;
            auto& event = *eventOpt;

            // pass the event to imgui to be parsed

            ImGui::SFML::ProcessEvent(window, event);

            // this event triggers when the window is closed
            if (event.is<sf::Event::Closed>())
            {
                window.close();
            }

            // this event is triggered when a key is pressed
            if (auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) 
            {
                if (keyEvent->scancode == sf::Keyboard::Scancode::Escape)
                    window.close();
            }

            if (auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
                if (keyEvent->scancode == sf::Keyboard::Scancode::X) 
                {
                    float newRadius = 30.0f; // default radius
                    sf::Vector2f startPos = {350.0f, 100.0f}; // starting position
                    sf::CircleShape newCircle(newRadius, 32);
                    newCircle.setPosition(startPos);
                    col = hexToRGB("FAF8F1");
                    newCircle.setFillColor(sf::Color{col.r, col.g, col.b});

                    CircleData cd;
                    cd.shape = newCircle;
                    cd.radius = newRadius;
                    cd.velocity = {2.0f, -1.5f}; // default speed
                    dynamicCircles.push_back(cd);
                }
            }

            if (auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) 
            {
                if (keyEvent->scancode == sf::Keyboard::Scancode::C){
                    dynamicCircles.erase(dynamicCircles.begin());
                }
                    
            }
        }

        // update imgui for this frame with the time that the last frame took
        sf::Time dt_gui = deltaClock.restart(); 
        ImGui::SFML::Update(window,dt_gui);
        float dt = dt_gui.asSeconds();

        // draw the UI

        ImGui::Begin("Window Title");
        ImGui::Text("Window Text!");
        ImGui::Checkbox("Draw Circle", &drawCircle);
        ImGui::SameLine();
        ImGui::Checkbox("Draw Text", &drawText);
        ImGui::SliderFloat("Radius", &circleRadius, 0.0f, 300.0f);
        ImGui::SliderInt("Sides", &circleSegments, 3, 64);
        ImGui::ColorEdit3("Colour Circle", c);
        ImGui::InputText("Input Text", displayString, 255);
        ImGui::SameLine();
        
        if (ImGui::Button("Reset Circle"))
        {
            circle.setPosition({50,50});
        }

        ImGui::End();

        sf::Vector2f circleCenter = circle.getPosition() + sf::Vector2f(circle.getRadius(), circle.getRadius());
        circle.setRadius(circleRadius);
        circle.setPointCount(circleSegments);
        circle.setPosition(circleCenter - sf::Vector2f(circleRadius, circleRadius));

        // Reset acceleration vector
        sf::Vector2f acceleration({0.f, 0.f});

        // Check key presses 
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W)) acceleration.y -= playerAcceleration; 
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S)) acceleration.y += playerAcceleration; 
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A)) acceleration.x -= playerAcceleration; 
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D)) acceleration.x += playerAcceleration;

            
        // Update velocity
        playerSpeed += acceleration;
        
        // Friction
        if (acceleration.x == 0.f && acceleration.y == 0.f) playerSpeed -= playerSpeed * playerFriction * dt;

        // Limit speed
        float speedLength = std::sqrt(playerSpeed.x * playerSpeed.x + playerSpeed.y * playerSpeed.y);
        if (speedLength > playerMaxSpeed) playerSpeed = (playerSpeed / speedLength) * playerMaxSpeed;

        // Move the player **once** per frame using delta time
        player.move(playerSpeed * dt);

        // // --- Debug ---
        // std::cout << "Pos: " << player.getPosition().x << ", " << player.getPosition().y
        //         << " | Speed: " << playerSpeed.x << ", " << playerSpeed.y << "\n";
     
        // basic animation move that each frame if it's still in frame
        circleSpeed *= 0.99f;     // 0.99 = 1% velocity lost per frame (or use dt scaling)
        circle2Speed *= 0.99f;
        circle3Speed *= 0.99f;

        std::cout << "address of circleSpeed: " << &circleSpeed << std::endl;
        std::cout << "Size of circle Speed: " << sizeof(circleSpeed) << " bytes" << std::endl;
        std::cout << "address of circleSpeed +1: " << &circleSpeed +1 << std::endl;
        std::cout << "type of circleSpeed: " << typeid(circleSpeed).name() << std::endl;
        std::cout << "char: " << sizeof(char) << " byte\n";
        std::cout << "int: " << sizeof(int) << " bytes\n";
        std::cout << "float: " << sizeof(float) << " bytes\n";
        std::cout << "double: " << sizeof(double) << " bytes\n";

        // Move circles based on their velocity (scaled by dt)
        circle.setPosition(circle.getPosition() + circleSpeed * dt);
        circle2.setPosition(circle2.getPosition() + circle2Speed * dt);
        circle3.setPosition(circle3.getPosition() + circle3Speed * dt);

        // collision detection
        collision(circle, circle2, circleRadius, circle2Radius, circleSpeed, circle2Speed, ballFriction);
        collision(circle, circle3, circleRadius, circle3Radius, circleSpeed, circle3Speed, ballFriction); 
        collision(circle3, circle2, circle3Radius, circle2Radius, circle3Speed, circle2Speed, ballFriction);

        collision(player, circle, playerRadius, circleRadius, playerSpeed, circleSpeed, ballFriction);
        collision(player, circle3, playerRadius, circle3Radius, playerSpeed, circle3Speed, ballFriction); 
        collision(player, circle2, playerRadius, circle2Radius, playerSpeed, circle2Speed, ballFriction);


        // reverse speed if touching the sides
        circleBoundaries(circle, circleSpeed.x, circleSpeed.y, circleRadius, wWidth, wHeight);
        circleBoundaries(circle2, circle2Speed.x, circle2Speed.y, circle2Radius, wWidth, wHeight);
        circleBoundaries(circle3, circle3Speed.x, circle3Speed.y, circle3Radius, wWidth, wHeight);

        circleBoundaries(player, playerSpeed.x, playerSpeed.y, playerRadius, wWidth, wHeight);

        // my struct circles

        for (auto& cd : dynamicCircles) 
        {
            // Move manually
            cd.shape.setPosition(cd.shape.getPosition() + cd.velocity * dt);
            
            // Collisions
            collision(cd.shape, circle, cd.radius, circleRadius, cd.velocity, circleSpeed, ballFriction);
            collision(cd.shape, circle2, cd.radius, circle2Radius, cd.velocity, circle2Speed, ballFriction);
            collision(cd.shape, circle3, cd.radius, circle3Radius, cd.velocity, circle3Speed, ballFriction);
                    
            for (size_t i = 0; i < dynamicCircles.size(); i++) 
            {
                collision(dynamicCircles[i].shape, player, dynamicCircles[i].radius, playerRadius, dynamicCircles[i].velocity, playerSpeed, ballFriction);
                for (size_t j = i + 1; j < dynamicCircles.size(); j++) {
                    collision(dynamicCircles[i].shape, dynamicCircles[j].shape, dynamicCircles[i].radius, dynamicCircles[j].radius, dynamicCircles[i].velocity, dynamicCircles[j].velocity, ballFriction);
                }
            }

            cd.velocity *= 0.99f;
            // Boundary check
            circleBoundaries(cd.shape, cd.velocity.x, cd.velocity.y, cd.radius, wWidth, wHeight);

        }

        myText2.setString(displayString);

        window.clear(); // clear the window of anything previously drawn
        if (drawCircle)
        {
            window.draw(circle);
            window.draw(circle2);
            window.draw(circle3);
            window.draw(player);
            for (auto& cd : dynamicCircles) 
            {
                window.draw(cd.shape);
            }
        }

        if (drawText) // draw the text if the boolean is true
        {
            window.draw(myText);
            window.draw(myText2);
        }
        ImGui::SFML::Render(window);
        window.display(); // call the window display function
    }

    return 0;
}

