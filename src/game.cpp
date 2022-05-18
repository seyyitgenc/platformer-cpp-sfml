#include <SFML/Graphics.hpp>
#include <iostream>
void gameBuild()
{
    float dt;
    sf::Clock clock;

    const float gridSize = 50.f;

    sf::RenderWindow window(sf::VideoMode(1024, 768), "Hu-hunt");
    window.setFramerateLimit(120);
    // player
    bool canJump = true;
    const float movementSpeed = 300.f;
    const float gravity = 10.f;
    const float tVelocity = 200.f;
    const float jumpSpeed = 200.f;
    sf::Vector2f velocity;
    sf::RectangleShape player;
    player.setSize(sf::Vector2f(gridSize, gridSize));
    player.setFillColor(sf::Color::Red);
    // enemy
    while (window.isOpen())
    {
        dt = clock.restart().asSeconds();
        sf::Event event;
        // gravity
        velocity.y += gravity * dt;
        // terminal velocity
        if (velocity.y > tVelocity * dt)
        {
            velocity.y = tVelocity * dt;
        }
        std::cout << velocity.y << std::endl;
        // event loop
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
        velocity.x = 0.f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && canJump == true)
        {
            velocity.y = 0.f;
            velocity.y += -jumpSpeed * dt;
            player.move(0, velocity.y);
            canJump = false;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            canJump = true;
            // nothing for now
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            velocity.x -= movementSpeed * dt;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            velocity.x += movementSpeed * dt;
        }
        player.move(velocity);
        // drawing
        window.clear();
        window.draw(player);
        window.display();
    }
}