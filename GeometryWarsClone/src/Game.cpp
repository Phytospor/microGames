#include "Game.h"
#include <iostream>
#include <fstream>
#include <filesystem>


Game::Game(const std::string & config)
    : m_text(m_font, "Default", 24)
{
    init(config);
}

std::shared_ptr<Entity> Game::player()
{
    return m_entities.getEntities("player").front();
}

void Game::init(const std::string & path)
{
    // file loader
    std::ifstream fin(path);
    std::string line;

    if (!fin) {
        // If the file cannot be opened, throw an exception
        throw std::runtime_error("Unable to open file");
    }

    while (std::getline(fin, line))
    {
        std::cout << line << std::endl; 
        std::istringstream iss(line);
        std::string token;
        iss >> token;

        if (token == "Window") 
        {   
            unsigned int windowWidth, windowHeight, frameRate, fullscreenFlag;
            iss >> windowWidth >> windowHeight >> frameRate >> fullscreenFlag;
            std::cout << windowWidth << "  " << windowHeight << std::endl; 
            m_window.create(sf::VideoMode({windowWidth, windowHeight}), "Geometry Wars Clone");
            m_window.setKeyRepeatEnabled(fullscreenFlag);
            m_window.setFramerateLimit(frameRate);
        } 

        

        else if (token == "Font") 
        {
            std::string fontFile;
            int fontSize, fontR, fontG, fontB;

            iss >> fontFile >> fontSize >> fontR >> fontG >> fontB;

            try
            {
                m_font = sf::Font(fontFile);
            }
            catch (const std::exception& e)
            {
                std::cerr << "Could not load font: " << e.what() << '\n';
                std::exit(-1);
            }

            m_text.setFont(m_font);
            m_text.setString("Geometry Wars Clone");
            m_text.setCharacterSize(fontSize);
            m_text.setFillColor(sf::Color(fontR, fontG, fontB));


        }
        
        else if (token == "Player") 
        {
            int shapeRadius, collisionRadius, fillR, fillG, fillB, outlineR, outlineG, outlineB, outlineThickness, vertices; // Shape Radius SR; Collision Radius CR; Speed S; Fill Colour FR,FG,FB; Outline Colour OR,OG,OB; Outline Thickness; Shape Vertices V
            float speed;

            iss >> shapeRadius >> collisionRadius >> speed >> fillR >> fillG 
            >> fillB >> outlineR >> outlineG >> outlineB >> outlineThickness 
            >> vertices;
            
            m_playerConfig = PlayerConfig();
            m_playerConfig.SR = shapeRadius;
            m_playerConfig.CR = collisionRadius;
            m_playerConfig.S = speed;
            m_playerConfig.FR = fillR;
            m_playerConfig.FG = fillG;
            m_playerConfig.FB = fillB;
            m_playerConfig.OR = outlineR;
            m_playerConfig.OG = outlineG;
            m_playerConfig.OB = outlineB;
            m_playerConfig.OT = outlineThickness;
            m_playerConfig.V = vertices;

        }
        
        else if (token == "Enemy") 
        {
            
            int shapeRadius, collisionRadius, outlineR, 
                outlineG, outlineB, outlineThickness, verticeMin, verticeMax,
                smallLifespan, spawnInterval; 
            float speedMin, speedMax;

            iss >> shapeRadius >> collisionRadius >> speedMin >> speedMax 
            >> outlineR >> outlineG >> outlineB >> outlineThickness 
            >> verticeMin >> verticeMax >> smallLifespan >> spawnInterval;

            m_enemyConfig = EnemyConfig();
            m_enemyConfig.SR = shapeRadius;
            m_enemyConfig.CR = collisionRadius;
            m_enemyConfig.SMIN = speedMin;
            m_enemyConfig.SMAX = speedMax;
            m_enemyConfig.OR = outlineR;
            m_enemyConfig.OG = outlineG;
            m_enemyConfig.OB = outlineB;
            m_enemyConfig.OT = outlineThickness;
            m_enemyConfig.VMIN = verticeMin;
            m_enemyConfig.VMAX = verticeMax;
            m_enemyConfig.L = smallLifespan;
            m_enemyConfig.SI =  spawnInterval;
        }  

        else if (token == "Bullet") 
        {
            int shapeRadius, collisionRadius, fillR, fillG, fillB,
                outlineR, outlineG, outlineB, outlineThickness, shapeVertices,
                lifeSpan;
            float speed;
            
            iss >> shapeRadius >> collisionRadius >> speed >> fillR >> fillG >> fillB 
            >> outlineR >> outlineG >> outlineB >> outlineThickness >> shapeVertices 
            >> lifeSpan;

            m_bulletConfig = BulletConfig();
            m_bulletConfig.SR = shapeRadius;
            m_bulletConfig.CR = collisionRadius;
            m_bulletConfig.FR = fillR;
            m_bulletConfig.FG = fillG;
            m_bulletConfig.FB = fillB;
            m_bulletConfig.OR = outlineR;
            m_bulletConfig.OG = outlineG;
            m_bulletConfig.OB = outlineB;
            m_bulletConfig.OT = outlineThickness;
            m_bulletConfig.S = speed;
            m_bulletConfig.V = shapeVertices;
            m_bulletConfig.L = lifeSpan;
        } 
    }

    // use the premade PlayerConfig, EnemyConfig, BulletConfig variables

    // set up default window parameter - change that to values in config file

    if(!ImGui::SFML::Init(m_window)){}

    // scale the imgui ui and text size by 2
    ImGui::GetStyle().ScaleAllSizes(2.0f);
    ImGui::GetIO().FontGlobalScale = 2.0f;

    spawnPlayer();
    m_deltaClock = sf::Clock(); 
}



void Game::run()
{
    // todo: add pause functionality in here.
    // some systems should funciton while paused (rendering)
    // some systems shouldn't (movement/input)

    while (m_window.isOpen())
    {
        //update the entity manager
        if (m_paused)
        {
            sUserInput();
            sRender();
        }
        else
        {
            m_entities.update();

            ImGui::SFML::Update(m_window, m_deltaClock.restart());
            sf::Time dt = m_deltaClock.restart();
            float deltaTime = dt.asSeconds();

            sUserInput();
            sEnemySpawner();
            sMovement();
            sCollision();
            sGUI();
            sRender();
        }


        // increment the current frame
        // may need to be moved when pause implementd like if paused do that, else...

        m_currentFrame++;

    }

}



void Game::setPaused(bool paused)
{
    m_paused = paused;
}


// respawn the player in the middle of the screen

void Game::spawnPlayer()
{
    // ToDo: Finish adding all properties of the player with the correct values from the config file
    
    // we create every entity by calling EntityManager.addEntity(tag)
    // this returns a std::shared_ptr<Entity>, so we use 'auto' to save typing
    auto p = m_entities.addEntity("player");

    // Give this entity a Transform so it spawns at (200,200) with velocity (1,1,) and angle 0?

    p->add<CTransform>(
                    Vec2(m_window.getSize().x/2.0f, m_window.getSize().y/2.0f),
                    Vec2(m_playerConfig.S, m_playerConfig.S),
                    0);

    // the entity's shape will have radius 32, 8 sided, dark grey fill, red outline of ?
    p->add<CShape>(m_playerConfig.SR,m_playerConfig.V,
                    sf::Color(m_playerConfig.FR,m_playerConfig.FG,m_playerConfig.FB), 
                    sf::Color(m_playerConfig.OR,m_playerConfig.OG,m_playerConfig.OB), 
                    m_playerConfig.OT);

    // Add an input component to the player so that we can use inputs
    p->add<CInput>();

    // add collisions
    p->add<CCollision>(m_playerConfig.CR);

}


void Game::spawnEnemy()
{
    // TO DO: make sure the enemy is spawned properly with the m_enemyConfig variables
    // the enemy mus t be spawned completely within the bounds of the window
    auto e = m_entities.addEntity("enemy");

    float x = m_enemyConfig.SR + rand() % (m_window.getSize().x - 2 * m_enemyConfig.SR);
    float y = m_enemyConfig.SR + rand() % (m_window.getSize().y - 2 * m_enemyConfig.SR);

    float angle = ((float)rand() / (float)RAND_MAX) * 2.0f * 3.14159265f;
    Vec2<float> dir(std::cos(angle), std::sin(angle));

    float speed = ((float)rand() / (float)RAND_MAX) * (m_enemyConfig.SMAX - m_enemyConfig.SMIN) + m_enemyConfig.SMIN;
    Vec2<float> velocity = dir * speed;

    e->add<CTransform>(
        Vec2(x,y), 
        Vec2(velocity.x,velocity.y), 
        0.f);

    int vertices = m_enemyConfig.VMIN + rand() % (m_enemyConfig.VMAX - m_enemyConfig.VMIN + 1);

    // random fill color
    int fillR = rand() % 256;
    int fillG = rand() % 256;
    int fillB = rand() % 256;

    e->add<CShape>(
        m_enemyConfig.SR,vertices, 
        sf::Color(fillR,fillG,fillB),
        sf::Color(m_enemyConfig.OR,m_enemyConfig.OG,m_enemyConfig.OB),
        m_enemyConfig.OT);

    e->add<CCollision>(m_enemyConfig.CR);
    // record when the most recent enemy was spawned
    m_lastEnemySpawnTime = m_currentFrame;
}

void Game::sEnemySpawner()
{
    // TO DO: make sure the enemy is spawned properly with the m_enemyConfig variables
    // the enemy mus t be spawned completely within the vounds of the window


    //Spawn an enemy every SPAWN_INTERVAL milliseconds
    if (m_enemyClock.getElapsedTime().asMilliseconds() >= m_enemyConfig.SI)
    {
        spawnEnemy();
        m_enemyClock.restart();  //Reset clock so we can countdown to next enemy spawn
    }

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
    auto b = m_entities.addEntity("bullet");

    Vec2<float> spawnPoint = player()->get<CTransform>().pos;


    b->add<CTransform>(
                    spawnPoint,
                    (target-spawnPoint).normalise()*m_bulletConfig.S,
                    0);

    b->add<CShape>(
        m_bulletConfig.SR,
        m_bulletConfig.V,
        sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB),
        sf::Color(m_bulletConfig.OR, m_bulletConfig.OG, m_bulletConfig.OB),
        m_bulletConfig.OT);
    
    b->add<CCollision>(
        m_bulletConfig.CR
    );

    b->add<CLifespan>(
        m_bulletConfig.L
    );
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
    auto& pt = player()->get<CTransform>();
    auto& input = player()->get<CInput>();

    Vec2<float> dir(0.0f, 0.0f);

    if (input.up)    { dir.y -= 1.0f;}
    if (input.down)  { dir.y += 1.0f; }
    if (input.left)  { dir.x -= 1.0f; }
    if (input.right) { dir.x += 1.0f; }

    if (dir.x != 0.0f || dir.y != 0.0f)
    {
        pt.velocity = dir.normalise() * m_playerConfig.S;
    }
    else
    {
        pt.velocity = Vec2<float>(0.0f, 0.0f);
    }

    for (auto e : m_entities.getEntities())
    {
        if (!e->has<CTransform>())
        {
            continue;
        }

        auto& t = e->get<CTransform>();
        t.pos += t.velocity;
    }
    
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
    for (auto & p : m_entities.getEntities("player"))
    {
        if (p->has<CTransform>())
        {
            continue;
        }
    }

    // enemies vs enemies

    auto enemies = m_entities.getEntities("enemy");

    for (size_t i = 0; i < enemies.size(); i++)
    {
        for (size_t j = i + 1; j < enemies.size(); j++)
        {
            auto& a = enemies[i];
            auto& b = enemies[j];

            if (!a->has<CTransform>() || !a->has<CCollision>() ||
                !b->has<CTransform>() || !b->has<CCollision>())
            {
                continue;
            }

            auto& ta = a->get<CTransform>();
            auto& tb = b->get<CTransform>();
            auto& ca = a->get<CCollision>();
            auto& cb = b->get<CCollision>();

            // calculate distance
            Vec2<float> diff = tb.pos - ta.pos;
            float distSq = diff.x * diff.x + diff.y * diff.y;

            // calculate radius
            float radiiSum = ca.radius + cb.radius;

            // compare and reverse if true
            
            if (distSq < radiiSum * radiiSum)
            {
            float distance = std::sqrt(distSq);

            Vec2<float> normal;
            if (distance == 0.0f)
            {
                normal = Vec2<float>(1.0f, 0.0f);
                distance = .0001f;
            }
            else
            {
                normal = diff / distance;
            }

            // Relative velocity
            Vec2<float> relativeVelocity = tb.velocity - ta.velocity;
            float velAlongNormal = relativeVelocity.x * normal.x + relativeVelocity.y * normal.y;

            //If already moving apart, skip impulse response

            if (velAlongNormal > 0.0f)
            {
                continue;
            }

            // Positional correction
            float overlap = radiiSum - distance;
            float correctionPercent = 0.2f;
            Vec2<float> correction = normal * (overlap * 0.5f * correctionPercent);

            ta.pos -= correction;
            tb.pos += correction;

            // Elastic collision impulse (equal mass)
            float restitution = 0.5f;
            float impulseMagnitude = -(1.0f + restitution) * velAlongNormal / 2.0f;

            Vec2<float> impulse = normal * impulseMagnitude;

            ta.velocity -= impulse;
            tb.velocity += impulse;
            }
        }   
    }

    
    for (auto b : m_entities.getEntities("bullet"))
    {

        // do collision logic
        auto bulletPos = b->get<CTransform>().pos;
        auto bulletRadius = b->get<CCollision>().radius;

        for (auto e : m_entities.getEntities("enemy"))
        {
            auto enemyPos = e->get<CTransform>().pos;
            auto enemyRadius = e->get<CCollision>().radius;

            if (enemyPos.dist(bulletPos) <= enemyRadius + bulletRadius)
            {
                spawnSmallEnemies(e);
                e->destroy();
                b->destroy();
                break;
            }

        }

        for (auto e: m_entities.getEntities("smallEnemy"))
        {
            // do collision logic
        }
    }

    // all entities vs window border

    for (auto & entity : m_entities.getEntities())
    {

        if (entity->tag() == "bullet")
        {

        }
        // skip if entity does not have collide component
        if (!entity->has<CCollision>() || !entity->has<CTransform>() || entity->tag() == "Player" || entity->tag() == "bullet")
        {
            continue;
        }

        auto & transform = entity->get<CTransform>();
        auto & collision = entity->get<CCollision>();

        if (transform.pos.x + collision.radius >= m_window.getSize().x)
        {
            if (transform.velocity.x > 0)
            {
                transform.velocity.x *= -1;
                transform.pos.x = m_window.getSize().x - collision.radius;
            }
        }

        if (transform.pos.x - collision.radius <=0)
        {
            if (transform.velocity.x < 0)
            {
                transform.velocity.x *= -1;
                transform.pos.x = collision.radius;
            }
        }

        if (transform.pos.y + collision.radius >= m_window.getSize().y)
        {
            if (transform.velocity.y > 0)
            {
                transform.velocity.y *= -1;
                transform.pos.y = m_window.getSize().y - collision.radius;
            }
        }

        if (transform.pos.y - collision.radius <=0)
        {
            if (transform.velocity.y < 0)
            {
                transform.velocity.y *= -1;
                transform.pos.y = collision.radius;
            }
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

    // 
    for (auto e : m_entities.getEntities())
    {
            e->get<CShape>().circle.setPosition(e->get<CTransform>().pos);
            e->get<CTransform>().angle += 1.0f;
            e->get<CShape>().circle.setRotation(sf::degrees(e->get<CTransform>().angle));
            m_window.draw(e->get<CShape>().circle);
    }

    // draw the entity's sf::CircleShape
    m_window.draw(player()->get<CShape>().circle);

    m_window.draw(m_text);

    // draw the UI last

    ImGui::SFML::Render(m_window);
    
    m_window.display(

    );

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
                player()->get<CInput>().up= true;  
            } 
            if (keyPressed->scancode == sf::Keyboard::Scancode::S)
            {
                // TODO: set player's input componetn up to true
                std::cout << "S key Pressed\n";
                player()->get<CInput>().down = true;  
            } 
            if (keyPressed->scancode == sf::Keyboard::Scancode::D)
            {
                // TODO: set player's input componetn up to true
                std::cout << "D key Pressed\n"; 
                player()->get<CInput>().right = true;  
            }  
            if (keyPressed->scancode == sf::Keyboard::Scancode::A)
            {
                // TODO: set player's input componetn up to true
                std::cout << "A key Pressed\n";
                player()->get<CInput>().left = true;  
            }

            if (keyPressed->scancode == sf::Keyboard::Scancode::Space)
            {
                // TODO: set player's input componetn up to true
                std::cout << "Space key Pressed\n";
                Vec2<float> target(
                    sf::Mouse::getPosition(m_window).x,
                    sf::Mouse::getPosition(m_window).y
                    );
                spawnBullet(player(), target);
            }   
            

            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
            {
                std::exit(0);
            } 
        }

        if (const auto* keyPressed = event->getIf<sf::Event::KeyReleased>())
        {
            // print the key that was released to the console
            std::cout << "Key released = " << int(keyPressed->scancode) << "\n";

           if (keyPressed->scancode == sf::Keyboard::Scancode::W)
            {
                // TODO: set player's input componetn up to true
                std::cout << "W key released\n";
                player()->get<CInput>().up= false;  
            } 
            if (keyPressed->scancode == sf::Keyboard::Scancode::S)
            {
                // TODO: set player's input componetn up to true
                std::cout << "S key released\n";
                player()->get<CInput>().down = false;  
            } 
            if (keyPressed->scancode == sf::Keyboard::Scancode::D)
            {
                // TODO: set player's input componetn up to true
                std::cout << "D key released\n"; 
                player()->get<CInput>().right = false;  
            }  
            if (keyPressed->scancode == sf::Keyboard::Scancode::A)
            {
                // TODO: set player's input componetn up to true
                std::cout << "A key released\n";
                player()->get<CInput>().left = false;  
            }
            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
            {
            std::exit(0);
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

