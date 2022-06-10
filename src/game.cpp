#include <SFML/Graphics.hpp>
#include <iostream>
#include "window.hpp"
#include "collision.hpp"
#include "player.hpp"

void gameBuild()
{
    float dt;
    sf::Clock clock;
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Hu-hunt");
    window.setFramerateLimit(120);
    // player
    Player pl;
    pl.Create();

    // Bullet
    sf::RectangleShape bullet;
    bullet.setSize(sf::Vector2f(50, 10));
    bullet.setFillColor(sf::Color::Magenta);
    bullet.setPosition(sf::Vector2f(WIDTH, HEIGHT - 100));
    const float bulletSpeed = 100;
    sf::Vector2f bulletVelocity;
    sf::FloatRect nexPosBullet;
    while (window.isOpen())
    {
        // clock
        nexPosBullet = bullet.getGlobalBounds();
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

        bulletVelocity.x = 0.f;
        bulletVelocity.y = 0.f;
        bulletVelocity.x -= bulletSpeed * dt;
        bullet.move(bulletVelocity);

        pl.Update(dt);
        window.clear();
        for (const auto &r : vRects)
            createShape(r.size, r.pos, r.color, &window);
        window.draw(bullet);
        window.display();
    }
}