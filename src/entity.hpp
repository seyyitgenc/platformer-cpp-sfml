#include <SFML/Graphics.hpp>
#include <iostream>
#include "window.hpp"

const float gravity = 10.f;
const float tVelocity = 300.f;
const float movementSpeed = 300.f;

class enemy : public sf::RectangleShape
{
public:
    sf::Vector2f enemyVelocity;

private:
};
class Player : public sf::Drawable, public sf::Transformable
{
public:
    bool canJump = true;
    const float jumpSpeed = 300.f;
    Player()
    {
        player.setSize(sf::Vector2f(gridSize, gridSize));
        player.setFillColor(sf::Color::Red);
    }
    void Update(float &dt)
    {
        velocity.x = 0.f;
        // gravity
        velocity.y += gravity * dt;
        if (velocity.y > tVelocity * dt)
            velocity.y = tVelocity * dt;

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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            // nothing will happen for now
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            velocity.x += -movementSpeed * dt;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            velocity.x += movementSpeed * dt;
        }

        player.move(velocity);
    }

private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        target.draw(player, states);
    }
    sf::Vector2f velocity;
    sf::RectangleShape player;
};