#include <iostream>
#include <cmath>    // for std::sqrt
#include <algorithm> // for std::swap
#include <memory>
#include <fstream>
#include <filesystem>

#include <SFML/Graphics.hpp>
#include <imgui.h>
#include "../libraries/ImGui-SFML/imgui-SFML.h"

// example for file loader:

void loadFromFile(const std::string& filename)
{
    std::ifstream fin(filename);
    std::string first, last;
    int id;
    float avg;

    while (fin >> first)
    {
        fin >> last >> id >> avg; 
        //the above is basically:
        // fin >> last;
        // fin >> id;
        // fin >> avg;

    // do something with first, last, id, avg
    }
}


// create classes and functions in different header files or keep here in from of the main function

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

// prep everything for file loader:

struct Shape {
    std::string type;
    std::string name;
    float x, y;
    float vx, vy;
    int r, g, b;
    float size; // either radius in circle or width in rectangle
    float height; // for rectangle
    bool draw = true;
};


struct FontConfig {
    std::string filename;
    int size;
    int r, g, b;
};

struct Window {
    int width, height;
};

// Function to load data from a file
void loadFromFile(const std::string& filename, Window& window, FontConfig& font, std::vector<Shape>& shapes) {
    // Open the file for reading
    std::ifstream fin(filename);
    if (!fin) {
        // If the file cannot be opened, throw an exception
        throw std::runtime_error("Unable to open file");
    }

    // Read the file line by line
    std::string line;
    while (std::getline(fin, line)) {
        // Split the line into tokens
        std::istringstream iss(line);
        std::string token;

        // Check the type of data in the line
        if (line.find("Window") == 0) {
            // If the line starts with "Window", read the window data
            iss >> token; // Window
            iss >> window.width >> window.height;
            std::cout << "Window: " << window.width << "x" << window.height << std::endl;
        } 
        else if (line.find("Font") == 0) {
            // If the line starts with "Font", read the font data
            iss >> token; // Font
            iss >> font.filename >> font.size >> font.r >> font.g >> font.b;
            std::cout << "Font: " << font.filename << ", size: " << font.size << ", colour: (" << font.r << ", " << font.g << ", " << font.b << ")" << std::endl;
        } 
        else if (line.find("Circle") == 0) {

            // If the line starts with "Circle", read the circle data
            Shape shape;
            iss >> token; // Circle

            shape.type = token;
            iss >>  shape.name >> 
                    shape.x >> shape.y >> 
                    shape.vx >> shape.vy >> 
                    shape.r >> shape.g >> shape.b >>
                    shape.size;

        
            shapes.push_back(shape);
            std::cout << "Circle: " << shape.name 
            << ", position: (" << shape.x << ", " << shape.y 
            << "), velocity: (" << shape.vx << ", " << shape.vy 
            << "), color: (" << shape.r << ", " << shape.g << ", " << shape.b 
            << "), Radius: " << shape.size << std::endl;
        } 
        
        else if (line.find("Rectangle") == 0) {
            // If the line starts with "Rectangle", read the rectangle data
            Shape shape;
            iss >> token; // Rectangle
            shape.type = token;

            iss >>  shape.name >> 
                    shape.x >> shape.y >> 
                    shape.vx >> shape.vy >> 
                    shape.r >> shape.g >> shape.b >> 
                    shape.size >> shape.height;
            shapes.push_back(shape);
            std::cout   << "Rectangle: " << shape.name 
                        << ", position: (" << shape.x << ", " << shape.y 
                        << "), velocity: (" << shape.vx << ", " << shape.vy 
                        << "), color: (" << shape.r << ", " << shape.g << ", " << shape.b 
                        << "), length: " << shape.size 
                        << ", width: " << shape.height << std::endl;
        }
    }
    // Close the file
    fin.close();
}


int main(int argc, char const *argv[])
{   

    // create a new window of size w*h pixels
    // top-left of the window is (0,0) and bottom-right is (w,h)
    // you will have to read these from the config file -> figure out how to read in config files.
    // also, get config file from the video
    // Create a Window instance

    Window wind;
    // Create a Font instance
    FontConfig fontConfig;
    // Create a vector to hold Shape instances
    std::vector<Shape> shapes;

    // Load data from a file
    loadFromFile("assets/config_file.txt", wind, fontConfig, shapes);

    sf::RenderWindow window(sf::VideoMode({static_cast<unsigned int>(wind.width), static_cast<unsigned int>(wind.height)}), "Bouncy Shapes");
    window.setFramerateLimit(60); // limits frame rate to 60fps

    std::filesystem::path exePath = std::filesystem::current_path(); // current working directory
    sf::Font font;

    std::cout << typeid(font).name() << std::endl;
    if (!font.openFromFile((exePath / fontConfig.filename).string())) 
    {
        throw std::runtime_error("Failed to load font!");
    }

    // Create a vector to store whether each shape should be drawn

    std::vector<Shape> initShapes = shapes;

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
    

    // setup character array to set the text - this can be changed in the ImGui window
    char displayString[255] = "Sample Text";

    // set up the text object that will be drawn to the screen
    std::vector<float> textPosition = {100.f, 100.f};

    sf::Text displayText(font);      // font loaded previously
    displayText.setCharacterSize(fontConfig.size);
    displayText.setFillColor(sf::Color(col.r, col.g, col.b));  // use your chosen color
    displayText.setPosition({textPosition[0],textPosition[1]});

    int selectedShape = 0;
    float speedFactor = 1.0f;

    // main loop - continues for each frame while window is open
    while (window.isOpen())
    {
        window.clear();
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

            if (auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) 
            {
                if (keyEvent->scancode == sf::Keyboard::Scancode::X) 
                {

                }
            }

            if (auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) 
            {

            }
        }

        // update imgui for this frame with the time that the last frame took
        sf::Time dt_gui = deltaClock.restart(); 
        ImGui::SFML::Update(window,dt_gui);

        float dt = dt_gui.asSeconds();

        // draw the UI

        ImGui::Begin("I am the Shapes");
        ImGui::Text("Shapes Loaded: %zu", shapes.size());

        static std::vector<const char*> shapesList;
        shapesList.clear();
        shapesList.reserve(shapes.size());

        for (auto& s : shapes)
            shapesList.push_back(s.name.c_str());


        ImGui::Combo("Shapes", &selectedShape, shapesList.data(), static_cast<int>(shapesList.size()));

        if (selectedShape >= 0 && selectedShape < static_cast<int>(shapes.size())) 
        {
            Shape& shape = shapes[selectedShape];

            ImGui::Checkbox("Draw", &shape.draw);

            // Position sliders
            ImGui::SliderFloat("X", &shape.x, 0.f, static_cast<float>(wind.width));
            ImGui::SliderFloat("Y", &shape.y, 0.f, static_cast<float>(wind.height));

            // Velocity sliders
            ImGui::SliderFloat("Velocity X", &shape.vx, -500.f, 500.f);
            ImGui::SliderFloat("Velocity Y", &shape.vy, -500.f, 500.f);

            //speed factor slider
            ImGui::SliderFloat("Speed Factor", &speedFactor, -100.f, 100.f);
            // Size sliders
            
            if (shape.type == "Rectangle") 
            {
                ImGui::SliderFloat("Width", &shape.size, 1.f, 500.f);
                ImGui::SliderFloat("Height", &shape.height, 1.f, 500.f);
            }

            if (shape.type == "Circle") 
            {
                ImGui::SliderFloat("Radius", &shape.size, 1.f, 500.f);
            }

            // Color editor
            float color[3] = { shape.r / 255.f, shape.g / 255.f, shape.b / 255.f };
            if (ImGui::ColorEdit3("Color", color)) 
            {
                shape.r = static_cast<int>(color[0] * 255);
                shape.g = static_cast<int>(color[1] * 255);
                shape.b = static_cast<int>(color[2] * 255);
            }

            // Rename shape
            char buffer[64];

            std::strncpy(buffer, shape.name.c_str(), sizeof(buffer) - 1);

            buffer[sizeof(buffer) - 1] = '\0'; // ensure null termination

            if (ImGui::InputText("Rename", buffer, sizeof(buffer))) 
            {
                shape.name = buffer;
            }
        }
        ImGui::InputText("Input Text", displayString, 255);
        ImGui::SliderFloat("Text Position x", &textPosition[0],0.f, static_cast<float>(wind.width));
        ImGui::SliderFloat("Text Position y", &textPosition[1],0.f, static_cast<float>(wind.width));

        if (ImGui::Button("Reset Circle"))
        {
            shapes = initShapes;
        }

        ImGui::End();

        // Reset acceleration vector
        //sf::Vector2f acceleration({0.f, 0.f});

        // basic animation move that each frame if it's still in frame
        //circleSpeed *= 0.99f;     // 0.99 = 1% velocity lost per frame (or use dt scaling)

        
        // Draw the shapes loaded from the file
        for (size_t i = 0; i < shapes.size(); ++i) {
            if (!shapes[i].draw) continue;
            
            Shape& shape = shapes[i];

            shape.x += shape.vx * dt * speedFactor;
            shape.y += shape.vy * dt * speedFactor;


            if (shape.type == "Circle") {
                sf::CircleShape circleShape(shape.size);
                circleShape.setFillColor(sf::Color(shape.r, shape.g, shape.b));
                circleShape.setPosition(sf::Vector2f(shape.x, shape.y));
                window.draw(circleShape);

            } 
            else if (shape.type == "Rectangle") {
                sf::RectangleShape rectangleShape(sf::Vector2f(shape.size, shape.height));
                rectangleShape.setFillColor(sf::Color(shape.r, shape.g, shape.b));
                rectangleShape.setPosition(sf::Vector2f(shape.x, shape.y));
                window.draw(rectangleShape);
            }
            // Bounce horizontally
            if (shape.x < 0) { 
                shape.x = 0; 
                shape.vx = -shape.vx; 
            }
            else if (shape.type == "Circle" && shape.x + 2*shape.size > wind.width) { 
                shape.x = wind.width - 2*shape.size; 
                shape.vx = -shape.vx; 
            }
            else if (shape.type == "Rectangle" && shape.x + shape.size > wind.width) { 
                shape.x = wind.width - shape.size; 
                shape.vx = -shape.vx; 
            }

            // Bounce vertically
            if (shape.y < 0) { 
                shape.y = 0; 
                shape.vy = -shape.vy; 
            }
            else if (shape.type == "Circle" && shape.y + 2*shape.size > wind.height) { 
                shape.y = wind.height - 2*shape.size; 
                shape.vy = -shape.vy; 
            }
            else if (shape.type == "Rectangle" && shape.y + shape.height > wind.height) { 
                shape.y = wind.height - shape.height; 
                shape.vy = -shape.vy; 
            }

        }

        // draw text
        displayText.setPosition({textPosition[0],textPosition[1]});
        displayText.setString(displayString);
        window.draw(displayText);

        ImGui::SFML::Render(window);
        window.display(); // call the window display function
    }

    ImGui::SFML::Shutdown();
    return 0;
}
