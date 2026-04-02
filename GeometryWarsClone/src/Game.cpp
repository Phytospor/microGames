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

    // set up default window parameter - change that to values in config file

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
    return m_entities.getEntities("player").back(); // so player does not actually get destroyed, it will just go back to the middle if it exists.
}

void Game::run()
{
    // todo: add pause functionality in here.
    // some systems should funciton while paused (rendering)
    // some systems shouldn't (movement/input)

    while (true)
    {
        //updatte the entity manager
        m_entities.update();

        ImGui::SFML::Update(m_window, m_deltaClock.restart());

        sUserInput();
        sEnemySpawner();
        sMovement();
        sCollision();
        sGUI();
        sRender();

        // increment the current frame
        // may need to be moved when pause implementd like if paused do that, else...

        m_currentFrame++;

    }

}



void Game::setPaused(bool paused)
{
    // ToDo
    //or do it in game::run() function (like just set the variable)
}


// respawn the player in the middle of the screen

void Game::spawnPlayer()
{
    // ToDo: Finish adding all properties of the player with the correct values from the config file
    
    // we create every entity by calling EntityManager.addEntity(tag)
    // this returns a std::shared_ptr<Entity>, so we use 'auto' to save typing
    auto e = m_entities.addEntity("player");

    // Give this entity a Transform so it spawns at (200,200) with velocity (1,1,) and angle 0?

    e->add<CTransform>(Vec2(200.0f,200.0f), Vec2(1.0f,1.0f), 0.0f);

    // the entity's shape will have radius 32, 8 sided, dark grey fill, red outline of ?
    e->add<CShape>(32.f,8,sf::Color(10,10,10), sf::Color(255,0,0), 4.0f);
    // that will be something like: e->add<CShape>(m_playerConfig.SR,8,sf::Color(10,10,10), sf::Color(255,0,0), 4.0f);
    // Add an input component to the player so that we can use inputs
    e->add<CInput>();

}


void Game::spawnEnemy()
{
    // TO DO: make sure the enemy is spawned properly with the m_enemyConfig variables
    // the enemy mus t be spawned completely within the vounds of the window

    // record when the most recent enemy was spawned
    m_lastEnemySpawnTime = m_currentFrame;
}

void Game::sEnemySpawner()
{
    // TO DO: make sure the enemy is spawned properly with the m_enemyConfig variables
    // the enemy mus t be spawned completely within the vounds of the window

    // record when the most recent enemy was spawned
}


// spawns the small enemies when a big one (input entity e) explodes

void Game::spawnSmallEnemies(std::shared_ptr<Entity> e)
{
    // add property of smaller enemies: same as large ones with changed things

    // ToDo: spawn small enemies at the location of the input enemy e 

    // when we create the smaller enemy, we have to read the values of the original enemy
    // - spawn a number of small enemies equal to the vertices of the original enemy
    //      remember: if you want the original shape vertices count:
    //      e->get<CShape>().circle.getPointCount();
    // - set each small enemy to the sam ecolour as the original, half the size
    // - small enemies are worth double points of the original enemy

}

void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2<float>& target)
{
    // todo: implement the spawning of a bullet which travels toward target
    //      - bullet speed is given as a scalar speed
    //      - you must set the velocity by using formula in notes
}


void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
    //todo: implement your own special weapon
    // ideas: random bullets spawning around the map, with timer
}


void Game::sMovement()
{
    //ToDo: implement all entity movement in this function
    //      you should read the m_player->cInput component to determine the player 
    // add each velocity to the player

    // sample movment speed update for the player:
    auto& t = player()->get<CTransform>();
    t.pos.x += t.velocity.y;
    t.pos.y += t.velocity.y;
}


void Game::sLifespan()
{
    // ToDo: implement all lifespan functionality
    //
    // for all entities
    //      if entity has no lifspan component, skip it
    //      if entity has > 0 remaining lifespan, substract 1
    //      if it has lifespan and is alive
    //          scale its alpha channel properly (like the older it is the more opaque it gets)
    //      if it has lifspan and its time is up
    //          destroy the entity
}


void Game::sCollision()
{
    // ToDo:    implement all proper collisions between entities
    //          be sure to use the collision radius, not the shape radius
    // between player and enemies, between bullets and enemies, and the player and the walls, could also do enemies and walls.

    for (auto b : m_entities.getEntities("bullet"))
    {
        for (auto e : m_entities.getEntities("enemy"))
        {
            // do collision logic
        }

        for (auto e: m_entities.getEntities("smallEnemy"))
        {
            // do collision logic
        }
    }
}


void Game::sGUI()
{
    ImGui::Begin("Geometry Wars");

    ImGui::Text("Stuff Goes Here");

    ImGui::End();
}


void Game::sRender()
{
    if (!m_window.isOpen()) { return; }

    // ToDo:    change the code below to draw all of the entities
    //          sample drawing of the player Entity that we have created
    m_window.clear();

    // set the position of the shape based on the entity's transform->pos
    player()->get<CShape>().circle.setPosition(player()->get<CTransform>().pos);

    player()->get<CTransform>().angle += 1.0f;
    player()->get<CShape>().circle.setRotation(sf::degrees(player()->get<CTransform>().angle));

    // draw the entity's sf::CircleShape
    m_window.draw(player()->get<CShape>().circle);

    // draw the UI last

    ImGui::SFML::Render(m_window);

    m_window.display();

}


void Game::sUserInput()
{
    // toDo handle user input here
    //      not that you should only be setting hte player's input component variables here
    //      you should not implement the player's movement logic herre
    //      the movment logic system will read the variables you set in this function

    while (auto event = m_window.pollEvent())
    {
        // pass the event to imgui to be parsed
        ImGui::SFML::ProcessEvent(m_window, *event);

        // this event triggers when the window is closed
        if (event->is<sf::Event::Closed>())
        {
            std::exit(0);
        }

        // this event is triggered when a key is pressed
        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            // print the key that was pressed to the console
            std::cout << "Key pressed = " << int(keyPressed->scancode) << "\n";

            if (keyPressed->scancode == sf::Keyboard::Scancode::W)
            {
                // TODO: set player's input componetn up to true
                std::cout << "W key Pressed\n";  
            } 
        }

        if (const auto* keyPressed = event->getIf<sf::Event::KeyReleased>())
        {
            // print the key that was released to the console
            std::cout << "Key released = " << int(keyPressed->scancode) << "\n";

            if (keyPressed->scancode == sf::Keyboard::Scancode::W)
            {
                // ToDo: set player's input component up to false
                std::cout << "W Key Released\n";
            }
        }

        if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
        {
            Vec2<float> mpos(mousePressed->position.x, mousePressed->position.y);
            if (mousePressed->button == sf::Mouse::Button::Left)
            {
                //to do: call spawnBullet here
                std::cout << "Left mouse (" << mpos.x << ", " << ")\n";
            }
            else if (mousePressed->button == sf ::Mouse::Button::Right)
            {
                // to do: call special weapon here

                std::cout << "Right mouse (" << mpos.x <<", " << mpos.y << ")\n";
            }

        }

    }

}

