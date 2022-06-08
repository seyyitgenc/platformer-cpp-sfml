#pragma once
#include <SFML/Graphics.hpp>

struct rect
{
    sf::Vector2f size;
    sf::Vector2f pos;
    sf::Vector2f vel;
    sf::Color color;
};

void createShape(sf::Vector2f size, sf::Vector2f pos, sf::Color color, sf::RenderWindow *window)
{
    sf::RectangleShape shape;
    shape.setSize(size);
    shape.setPosition(pos);
    shape.setFillColor(color);
    window->draw(shape);
}