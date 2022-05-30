#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "window.hpp"

const float gravity = 10.f;
const float tVelocity = 300.f;
const float movementSpeed = 1200.f;

class enemy : public sf::RectangleShape
{
public:
    sf::Vector2f enemyVelocity;

private:
};

class Entity : public sf::Drawable, public sf::Transformable
{
public:
    sf::Vector2f velocity;
    sf::RectangleShape rect;
    bool canJump = true;
    const float jumpSpeed = 300.f;
    Entity()
    {
        rect.setSize(sf::Vector2f(gridSize, gridSize));
        rect.setFillColor(sf::Color::Red);
    }
    void Update(float &dt)
    {
        velocity.x = 0.f;
        velocity.y = 0.f;
        // gravity
        // velocity.y += gravity * dt;
        // if (velocity.y > tVelocity * dt)
        //    velocity.y = tVelocity * dt;

        // player movement
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            // if (canJump == true)
            //{
            //     velocity.y = 0.f;
            //     velocity.y -= jumpSpeed * dt;
            //     canJump = false;
            // }
            velocity.y -= movementSpeed * dt;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            velocity.y += movementSpeed * dt;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            velocity.x += -movementSpeed * dt;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            velocity.x += movementSpeed * dt;
        }
        rect.move(velocity);
    }

private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        target.draw(rect, states);
    }
};