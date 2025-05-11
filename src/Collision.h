#pragma once
#include <glm/glm.hpp>
#include <iostream>

struct AABB {
    glm::vec2 pos;   // center position
    glm::vec2 halfSize; // half-width and half-height
};
class Collision {
public:
static bool CheckAABBCollision(const AABB& a, const AABB& b);

static bool IsCollidingVertically(const glm::vec2& orbPos, const glm::vec2& orbSize,
                           const glm::vec2& platformPos, const glm::vec2& platformSize);
static bool IsCollidingHorizontally(const glm::vec2& currentPos, const glm::vec2& sizeA,
    const glm::vec2& wallPos, const glm::vec2& wallSize,
    const glm::vec2& previousPos);

};