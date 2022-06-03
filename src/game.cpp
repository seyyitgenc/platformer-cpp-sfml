#include <SFML/Graphics.hpp>
#include <iostream>
#include "entity.hpp"
#include "tilemap.hpp"
#include <vector>
#include <algorithm>
#include <cmath>

void createShape(sf::Vector2f size, sf::Vector2f pos, sf::Color color, sf::RenderWindow *window)
{
    sf::RectangleShape shape;
    shape.setSize(size);
    shape.setPosition(pos);
    shape.setFillColor(color);
    window->draw(shape);
}
struct rect
{
    sf::Vector2f size;
    sf::Vector2f pos;
    sf::Vector2f vel;
    std::array<rect *, 4> contact;
};
bool RectvsRect(const rect &r1, const rect &r2)
{
    return (r1.pos.x < r2.pos.x + r2.size.x && r1.pos.x + r1.size.x > r2.pos.x &&
            r1.pos.y < r2.pos.y + r2.size.y && r1.pos.y + r1.size.y > r2.pos.y);
}

bool RayVsRect(const sf::Vector2f &ray_origin, const sf::Vector2f &ray_dir, const rect *target, sf::Vector2f &contact_point, sf::Vector2f &contact_normal, float &t_hit_near)
{
    contact_normal = {0, 0};
    contact_point = {0, 0};

    // Cache division
    sf::Vector2f invdir = {1.0f / ray_dir.x, 1.0f / ray_dir.y};

    // Calculate intersections with rectangle bounding axes
    sf::Vector2f t_near = {(target->pos.x - ray_origin.x) * invdir.x, (target->pos.y - ray_origin.y) * invdir.y};
    sf::Vector2f t_far = {(target->pos.x + target->size.x - ray_origin.x) * invdir.x, (target->pos.y + target->size.y - ray_origin.y) * invdir.y};

    if (std::isnan(t_far.y) || std::isnan(t_far.x))
        return false;
    if (std::isnan(t_near.y) || std::isnan(t_near.x))
        return false;

    // Sort distances
    if (t_near.x > t_far.x)
        std::swap(t_near.x, t_far.x);
    if (t_near.y > t_far.y)
        std::swap(t_near.y, t_far.y);

    // Early rejection
    if (t_near.x > t_far.y || t_near.y > t_far.x)
        return false;

    // Closest 'time' will be the first contact
    t_hit_near = std::max(t_near.x, t_near.y);

    // Furthest 'time' is contact on opposite side of target
    float t_hit_far = std::min(t_far.x, t_far.y);

    // Reject if ray direction is pointing away from object
    if (t_hit_far < 0)
        return false;

    // Contact point of collision from parametric line equation
    contact_point = ray_origin + t_hit_near * ray_dir;

    if (t_near.x > t_near.y)
        if (invdir.x < 0)
            contact_normal = {1, 0};
        else
            contact_normal = {-1, 0};
    else if (t_near.x < t_near.y)
        if (invdir.y < 0)
            contact_normal = {0, 1};
        else
            contact_normal = {0, -1};

    // Note if t_near == t_far, collision is principly in a diagonal
    // so pointless to resolve. By returning a CN={0,0} even though its
    // considered a hit, the resolver wont change anything.
    return true;
}
bool DynamicRectVsRect(rect *r_dynamic, const float fTimeStep, rect &r_static,
                       sf::Vector2f &contact_point, sf::Vector2f &contact_normal, float &contact_time)
{
    // Check if dynamic rectangle is actually moving - we assume rectangles are NOT in collision to start
    if (r_dynamic->vel.x == 0 && r_dynamic->vel.y == 0)
        return false;

    // Expand target rectangle by source dimensions
    rect expanded_target;
    expanded_target.pos = r_static.pos - sf::Vector2f(r_dynamic->size.x / 2, r_dynamic->size.y / 2);
    expanded_target.size = r_static.size + r_dynamic->size;

    if (RayVsRect(r_dynamic->pos + sf::Vector2f(r_dynamic->size.x / 2, r_dynamic->size.y / 2), r_dynamic->vel * fTimeStep, &expanded_target, contact_point, contact_normal, contact_time))
        return (contact_time >= 0.0f && contact_time < 1.0f);
    else
        return false;
}
bool ResolveDynamicRectVsRect(rect *r_dynamic, const float fTimeStep, rect *r_static)
{
    sf::Vector2f contact_point, contact_normal;
    float contact_time = 0.0f;
    if (DynamicRectVsRect(r_dynamic, fTimeStep, *r_static, contact_point, contact_normal, contact_time))
    {
        if (contact_normal.y > 0)
            r_dynamic->contact[0] = r_static;
        else
            nullptr;
        if (contact_normal.x < 0)
            r_dynamic->contact[1] = r_static;
        else
            nullptr;
        if (contact_normal.y < 0)
            r_dynamic->contact[2] = r_static;
        else
            nullptr;
        if (contact_normal.x > 0)
            r_dynamic->contact[3] = r_static;
        else
            nullptr;

        r_dynamic->vel += sf::Vector2f(contact_normal.x * std::abs(r_dynamic->vel.x), contact_normal.y * std::abs(r_dynamic->vel.y)) * (1 - contact_time);
        return true;
    }

    return false;
}

class test
{
public:
    std::vector<rect> vRects;
    bool OnUserCreate()
    {
        vRects.push_back({{50.0f, 50.0f}, {10.0f, 40.0f}, {0.0f, 0.0f}});
        vRects.push_back({{170.0f, 70.0f}, {200.0f, 200.0f}, {0.0f, 0.0f}});
        vRects.push_back({{100.0f, 70.0f}, {300.0f, 270.0f}, {0.0f, 0.0f}});
        return true;
    }

    bool OnUserUpdate(float fElapsedTime, sf::RenderWindow *window)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            vRects[0].vel.y = -100.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            vRects[0].vel.y = +100.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            vRects[0].vel.x = -100.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            vRects[0].vel.x = +100.0f;
        // Draw all rectangles

        // Sort collisions in order of distance
        sf::Vector2f cp, cn;
        float t = 0, min_t = INFINITY;
        std::vector<std::pair<int, float>> z;

        // Work out collision point, add it to vector along with rect ID
        for (size_t i = 1; i < vRects.size(); i++)
        {
            if (DynamicRectVsRect(&vRects[0], fElapsedTime, vRects[i], cp, cn, t))
            {
                z.push_back({i, t});
            }
        }

        // Do the sort
        std::sort(z.begin(), z.end(), [](const std::pair<int, float> &a, const std::pair<int, float> &b)
                  { return a.second < b.second; });

        // Now resolve the collision in correct order
        for (auto j : z)
            ResolveDynamicRectVsRect(&vRects[0], fElapsedTime, &vRects[j.first]);
        // UPdate the player rectangles position, with its modified velocity
        vRects[0].pos += vRects[0].vel * fElapsedTime;
        return true;
    }

private:
};

void gameBuild()
{
    float dt;
    sf::Clock clock;
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Hu-hunt");
    window.setFramerateLimit(120);

    // player
    // collision test

    sf::Vector2f cp, cn;
    float ct;
    test t;
    t.OnUserCreate();
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
        window.clear();
        for (const auto &r : t.vRects)
            createShape(r.size, r.pos, sf::Color::Blue, &window);
        t.OnUserUpdate(dt, &window);
        window.display();
    }
}