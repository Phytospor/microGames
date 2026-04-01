#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include <stdio.h>

#include <SFML/Graphics.hpp>
#include <imgui.h>
#include "../libraries/ImGui-SFML/imgui-SFML.h"

#include "Game.cpp"


int main()
{
    Game g("assets/config.txt");
    g.run();
}