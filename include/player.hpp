#include "tilemap.hpp"
#include "collision.hpp"

const float movementSpeed = 300.0f;
const float gravity = 10.0f;
const float terminalVelocity = 500.0f;
const float jumpSpeed = 500.0f;
bool canJump = false;

class Player
{
public:
    Player()
    {
    }
    ~Player()
    {
    }
    bool Create()
    {
        load(vRects);
        return true;
    }
    bool Update(float fElapsedTime)
    {
        vRects[0].vel.x = 0.f;
        // gravity
        if (vRects[0].vel.y > terminalVelocity)
            vRects[0].vel.y = terminalVelocity;
        else
            vRects[0].vel.y += gravity;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && canJump == true)
        {
            vRects[0].vel.y = 0.f;
            vRects[0].vel.y = -jumpSpeed;
            canJump = false;
        }

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

                // if bottom collision occured player can jump
                if (vRects[0].pos.y < vRects[i].pos.y && vRects[0].pos.y + vRects[0].size.y < vRects[i].pos.y + vRects[i].size.y && vRects[0].pos.x < vRects[i].pos.x + vRects[i].size.x && vRects[0].pos.x + vRects[0].size.x > vRects[i].pos.x)
                {
                    canJump = true;
                }
            }
        }

        // Do the sort
        std::sort(z.begin(), z.end(), [](const std::pair<int, float> &a, const std::pair<int, float> &b)
                  { return a.second < b.second; });

        // Now resolve the collision in correct order
        for (auto j : z)
        {
            ResolveDynamicRectVsRect(&vRects[0], fElapsedTime, &vRects[j.first]);
        }
        for (size_t i = 0; i < movingPlatforms.size(); i++)
        {
            int index = movingPlatforms[i].second;
            if (vRects[index + 1].pos.x > WIDTH - vRects[index + 1].size.x)
            {
                isRight = false;
                isLeft = true;
            }
            else if (vRects[index + 1].pos.x < 0.f)
            {
                isRight = true;
                isLeft = false;
            }
            if (isRight == true)
            {
                vRects[index + 1].vel.x = +200;
            }
            else if (isLeft == true)
            {
                vRects[index + 1].vel.x = -200;
            }
            vRects[index + 1].pos += vRects[index + 1].vel * fElapsedTime;
        }
        // Update the player rectangles position, with its modified velocity
        vRects[0].pos += vRects[0].vel * fElapsedTime;

        return true;
    }

private:
    bool isRight = true;
    bool isLeft = false;
};