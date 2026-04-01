#include "Game.h"

#include <iostream>


Game::Game(const std::string & config)
    : m_text(m_font, "Default", 24)
{
    init(config);
}

void Game::init(const std::string & path)
{
    //todo: read in config file here
    // use the premade PlayerConfig, EnemyConfig, BulletConfig variables

    // set up default window parameter

    m_window.create(sf::VideoMode({1280,720}), "Geometry Wars Clone");
    m_window.setKeyRepeatEnabled(false);
    m_window.setFramerateLimit(60);

    if(!ImGui::SFML::Init(m_window)){}

    // scale the imgui ui and text size by 2
    ImGui::GetStyle().ScaleAllSizes(2.0f);
    ImGui::GetIO().FontGlobalScale = 2.0f;


    spawnPlayer();
}


std::shared_ptr<Entity> Game::player()
{
    return m_entities.getEntities("player").back();
}

void Game::run()
{
    // todo: add pause functionality in here.
    // some systems should funciton while paused (rendering)
}