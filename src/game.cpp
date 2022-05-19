#include <SFML/Graphics.hpp>
#include <iostream>

void gameBuild()
{
    float dt;
    sf::Clock clock;
    std::vector<sf::RectangleShape> walls;
    const float gridSize = 64.f;
    const unsigned int WIDTH = 1024;
    const unsigned int HEIGHT = 768;
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Hu-hunt");
    window.setFramerateLimit(60);

    // walls
    const int TileMap[] =
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

    // player
    bool canJump = true;
    const float movementSpeed = 300.f;
    const float gravity = 10.f;
    const float tVelocity = 300.f;
    const float jumpSpeed = 200.f;
    sf::Vector2f velocity;
    sf::RectangleShape player;
    player.setSize(sf::Vector2f(gridSize, gridSize));
    player.setFillColor(sf::Color::Red);

    // floor
    sf::RectangleShape floor;
    floor.setFillColor(sf::Color::Green);
    // tilemap
    float gridPos = -64.f;
    for (int i = 0; i < 192; ++i)
    {
        if (TileMap[i] == 1)
        {
            floor.setSize(sf::Vector2f(gridSize, gridSize));
            floor.setPosition(sf::Vector2f(gridSize + gridPos, 600));
            walls.push_back(floor);
            gridPos += 64.f;
        }
    }
    // enemy
    while (window.isOpen())
    {
        dt = clock.restart().asSeconds();
        sf::Event event;

        // event loop
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
        velocity.x = 0.f;
        // gravity
        velocity.y += gravity * dt;
        if (velocity.y > tVelocity * dt)
            velocity.y = tVelocity * dt;

        sf::FloatRect nexPos;

        // player movement
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            if (canJump == true)
            {
                velocity.y = 0.f;
                velocity.y -= jumpSpeed * dt;
                canJump = false;
            }
        }
        std::cout << dt << std::endl;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            velocity.y += movementSpeed * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            velocity.x += -movementSpeed * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            velocity.x += movementSpeed * dt;
        player.move(velocity);

        // floor collision
        for (auto &floor : walls)
        {
            sf::FloatRect playerBounds = player.getGlobalBounds();
            sf::FloatRect floorBounds = floor.getGlobalBounds();
            nexPos = playerBounds;
            nexPos.left += velocity.x * dt;
            nexPos.top += velocity.y * dt;
            if (floorBounds.intersects(nexPos))
            {
                // Bottom collision
                if (playerBounds.top < floorBounds.top && playerBounds.top + playerBounds.height < floorBounds.top + floorBounds.height && playerBounds.left < floorBounds.left + floorBounds.width && playerBounds.left + playerBounds.width > floorBounds.left)
                {
                    canJump = true;
                    player.setPosition(playerBounds.left, floorBounds.top - playerBounds.height);
                }
            }
        }
        // drawing
        window.clear();
        for (auto &i : walls)
        {
            window.draw(i);
        }
        window.draw(player);
        window.display();
    }
}