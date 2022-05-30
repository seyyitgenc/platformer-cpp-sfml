#include <SFML/Graphics.hpp>
#include <iostream>
#include "entity.hpp"
#include "tilemap.hpp"
#include <vector>
#include <algorithm>
bool PointvsRect(const sf::Vector2i &p, const sf::RectangleShape &r)
{
    return (p.x >= r.getPosition().x && p.y >= r.getPosition().y && p.x < r.getPosition().x + r.getSize().x && p.y < r.getPosition().y + r.getSize().y);
}

bool RectvsRect(const sf::RectangleShape &r1, const sf::RectangleShape &r2)
{
    return (r1.getPosition().x < r2.getPosition().x + r2.getSize().x && r1.getPosition().x + r1.getSize().x > r2.getPosition().x &&
            r1.getPosition().y < r2.getPosition().y + r2.getSize().y && r1.getPosition().y + r1.getSize().y > r2.getPosition().y);
}

bool RayvsRect(const sf::Vector2f &ray_origin, const sf::Vector2f &ray_dir, const sf::RectangleShape &target,
               sf::Vector2f &contact_point, sf::Vector2f &contact_normal, float &t_hit_near)
{
    contact_normal = {0, 0};
    contact_point = {0, 0};
    float t_nearX = (target.getPosition().x - ray_origin.x) / ray_dir.x;
    float t_farX = (target.getPosition().x + target.getSize().x - ray_origin.x) / ray_dir.x;
    float t_nearY = (target.getPosition().y - ray_origin.y) / ray_dir.y;
    float t_farY = (target.getPosition().y + target.getSize().y - ray_origin.y) / ray_dir.y;
    if (t_nearX > t_farX)
        std::swap(t_nearX, t_farX);
    if (t_nearY > t_farY)
        std::swap(t_nearY, t_farY);
    if (t_nearX > t_farY || t_nearY > t_farX)
        return false;
    t_hit_near = std::max(t_nearX, t_nearY);
    float t_hit_far = std::min(t_farX, t_farY);
    if (t_hit_far < 0)
        return false;
    contact_point = ray_origin + t_hit_near * ray_dir;
    if (t_nearX > t_nearY)
        (ray_dir.x < 0) ? contact_normal = {1, 0} : contact_normal = {-1, 0};
    else if (t_nearX < t_nearY)
        (ray_dir.y < 0) ? contact_normal = {0, 1} : contact_normal = {0, -1};
    return true;
}

bool DynamicRectvsRect(const Entity &in, const sf::RectangleShape &target, sf::Vector2f &contact_point, sf::Vector2f &contact_normal, float dt, float &contact_time)
{
    if (in.velocity.x == 0 && in.velocity.y == 0)
        return false;
    sf::RectangleShape expanded_Target;
    expanded_Target.setPosition(target.getPosition() - sf::Vector2f(in.rect.getSize().x / 2, in.rect.getSize().y / 2));
    expanded_Target.setSize(target.getSize() + in.rect.getSize());
    if (RayvsRect(in.rect.getPosition() + sf::Vector2f(in.rect.getSize().x / 2, in.rect.getSize().y / 2),
                  in.velocity * dt, expanded_Target, contact_point, contact_normal, contact_time))
    {
        if(contact_time<=1.0f)
            return true;
    }
    return false;
}

void gameBuild()
{
    float dt;
    sf::Clock clock;
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Hu-hunt");
    window.setFramerateLimit(120);

    std::vector<sf::RectangleShape> ColMap;

    // player
    Entity player;

    // collision test
    sf::RectangleShape rect;
    sf::RectangleShape mouseRect;
    sf::VertexArray ray(sf::Lines, 2);
    ray[0].position = sf::Vector2f(800, 60);
    ray[0].color = sf::Color::Blue;
    ray[1].color = sf::Color::Blue;

    sf::CircleShape circle(70);
    circle.setRadius(8);
    rect.setSize(sf::Vector2f(300, 100));
    rect.setPosition(sf::Vector2f(200, 400));
    mouseRect.setSize(sf::Vector2f(30, 100));
    mouseRect.setFillColor(sf::Color::Red);
    // loading TileMap
    TileMap _tileMap;
    _tileMap.load(ColMap);

    sf::Vector2f ray_direction;
    while (window.isOpen())
    {

        // mouse position
        sf::Vector2i vMouse = sf::Mouse::getPosition(window);
        // collision
        // PointvsRect(vMouse, rect) ? rect.setFillColor(sf::Color::Yellow) : rect.setFillColor(sf::Color::White);
        // RectvsRect(mouseRect, rect) ? rect.setFillColor(sf::Color::Yellow) : rect.setFillColor(sf::Color::White);
        // mouseRect.setPosition((sf::Vector2f)vMouse);
        ray[1].position = sf::Vector2f((sf::Vector2i)vMouse);
        ray_direction = ray[1].position - ray[0].position;
        sf::Vector2f cp, cn;
        float t;
        if (RayvsRect(ray[0].position, ray_direction, rect, cp, cn, t) && t <= 1.0f)
        {
            circle.setFillColor(sf::Color::Green);
            circle.setPosition(cp);
            rect.setFillColor(sf::Color::Yellow);
        }
        else
        {
            rect.setFillColor(sf::Color::White);
        }
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
        sf::FloatRect nexPos;
        player.Update(dt);

        // window collision
        //  left collision
        if (player.rect.getPosition().x < 0.f)
            player.rect.setPosition(0.f, player.rect.getPosition().y);
        // right collision
        if (player.rect.getPosition().x > WIDTH - player.rect.getGlobalBounds().width)
            player.rect.setPosition(WIDTH - player.rect.getGlobalBounds().width, player.rect.getPosition().y);
        // bottom collision
        if (player.rect.getPosition().y > HEIGHT - player.rect.getGlobalBounds().height)
            player.rect.setPosition(player.rect.getPosition().x, HEIGHT - player.rect.getGlobalBounds().height);
        // top collision
        if (player.rect.getPosition().y < 0.f)
            player.rect.setPosition(player.rect.getPosition().x, 0.f);

        // drawing
        window.clear();
        window.draw(player);
        window.draw(rect);
        window.draw(ray);
        window.draw(circle);
        window.display();
    }
}