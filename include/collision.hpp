#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <cmath>
#include "rectangle.hpp"

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
    {
        if (invdir.x < 0)
            contact_normal = {1, 0};
        else
            contact_normal = {-1, 0};
    }
    else if (t_near.x < t_near.y)
    {
        if (invdir.y < 0)
            contact_normal = {0, 1};
        else
            contact_normal = {0, -1};
    }
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
        r_dynamic->vel += sf::Vector2f(contact_normal.x * std::abs(r_dynamic->vel.x), contact_normal.y * std::abs(r_dynamic->vel.y)) * (1 - contact_time);
        return true;
    }

    return false;
}
