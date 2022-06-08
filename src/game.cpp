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
    while (window.isOpen())
    {
        // clock
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

        pl.Update(dt);

        window.clear();
        for (const auto &r : pl.vRects)
            createShape(r.size, r.pos, r.color, &window);
        window.display();
    }
}