#pragma once
#include "collision.hpp"

float movementSpeed = 1000.0f;
class Player
{
public:
    std::vector<rect> vRects;
    bool Create()
    {
        vRects.push_back({{50.0f, 50.0f}, {10.0f, 40.0f}, {0.0f, 0.0f}, sf::Color::Red});
        vRects.push_back({{10.0f, 300.0f}, {200.0f, 200.0f}, {0.0f, 0.0f}, sf::Color::Blue});
        vRects.push_back({{100.0f, 70.0f}, {300.0f, 270.0f}, {0.0f, 0.0f}, sf::Color::Green});
        return true;
    }
    bool Update(float fElapsedTime)
    {
        vRects[0].vel = {0.f, 0.f};
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            vRects[0].vel.y = -movementSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            vRects[0].vel.y = +movementSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            vRects[0].vel.x = -movementSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            vRects[0].vel.x = +movementSpeed;
        // Draw all rectangles

        // Sort collisions in order of distance
        sf::Vector2f cp, cn;
        float t = 0;
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