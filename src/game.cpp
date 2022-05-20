#include <SFML/Graphics.hpp>
#include <iostream>
#include "entity.hpp"
#include "tilemap.hpp"
void gameBuild()
{
    float dt;
    sf::Clock clock;
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Hu-hunt");
    window.setFramerateLimit(120);

    std::vector<sf::RectangleShape> walls;

    // player
    Player player;

    // enemy
    enemy enemy;
    // sf::RectangleShape enemy;
    enemy.setSize(sf::Vector2f(gridSize, gridSize));
    enemy.setFillColor(sf::Color::Blue);
    enemy.setPosition(sf::Vector2f(WIDTH - enemy.getGlobalBounds().width, HEIGHT - enemy.getGlobalBounds().height));

    // floor
    sf::RectangleShape floor;
    floor.setFillColor(sf::Color::Green);
    // platforms
    sf::RectangleShape platform;
    platform.setFillColor(sf::Color::Magenta);
    // tilemap
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (TileMap[i + j * 16] == 1)
            {
                floor.setSize(sf::Vector2f(gridSize, gridSize));
                floor.setPosition(sf::Vector2f(i * 64.f, j * 64.f));
                walls.push_back(floor);
            }
            if (TileMap[i + j * 16] == 2)
            {
                platform.setSize(sf::Vector2f(gridSize, gridSize));
                platform.setPosition(sf::Vector2f(i * 64.f, j * 64.f));
                walls.push_back(platform);
            }
        }
    }
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
        sf::FloatRect nexPos;

        player.Update(dt);

        
        // window collision
        // left collision
        //if (player.getPosition().x < 0.f)
        //    player.setPosition(0.f, player.getPosition().y);
        //// right collision
        //if (player.getPosition().x > WIDTH - player.getGlobalBounds().width)
        //    player.setPosition(WIDTH - player.getGlobalBounds().width, player.getPosition().y);
        //// bottom collision
        //if (player.getPosition().y > HEIGHT - player.getGlobalBounds().height)
        //    player.setPosition(player.getPosition().x, HEIGHT - player.getGlobalBounds().height);
        //// top collision
        //if (player.getPosition().y < 0.f)
        //    player.setPosition(player.getPosition().x, 0.f);
        //// floor collision
        //for (auto &floor : walls)
        //{
        //    sf::FloatRect playerBounds = player.getGlobalBounds();
        //    sf::FloatRect floorBounds = floor.getGlobalBounds();
        //    nexPos = playerBounds;
        //    nexPos.left += player.velocity.x * dt;
        //    nexPos.top += player.velocity.y * dt;
        //    if (floorBounds.intersects(nexPos))
        //    {
        //        // Bottom collision
        //        if (playerBounds.top < floorBounds.top && playerBounds.top + playerBounds.height < floorBounds.top + floorBounds.height && playerBounds.left < floorBounds.left + floorBounds.width && playerBounds.left + playerBounds.width > floorBounds.left)
        //        {
        //            player.canJump = true;
        //            player.setPosition(playerBounds.left, floorBounds.top - playerBounds.height);
        //        }
        //    }
        //}
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