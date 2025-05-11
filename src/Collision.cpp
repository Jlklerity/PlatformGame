#include "Collision.h"
 

bool Collision:: IsCollidingVertically(const glm::vec2& orbPos, const glm::vec2& orbSize,
                           const glm::vec2& platformPos, const glm::vec2& platformSize) {
    float orbBottom = orbPos.y;
    float orbTop = orbPos.y + orbSize.y;
    float platformBottom = platformPos.y;
    float platformTop = platformPos.y + platformSize.y;

    return orbBottom <= platformTop && orbTop >= platformBottom;
}
bool Collision:: IsCollidingHorizontally(const glm::vec2& currentPos, const glm::vec2& sizeA,
    const glm::vec2& wallPos, const glm::vec2& wallSize,
    const glm::vec2& previousPos) {


    float currentRight = currentPos.x;
    float previousRight = previousPos.x;

    float currentLeft = currentPos.x;
    float previousLeft = previousPos.x;

    bool verticallyOverlapping =
    currentPos.y + sizeA.y > wallPos.y &&
    currentPos.y < wallPos.y + wallSize.y;

    bool movingRight = currentPos.x > previousPos.x;
    bool movingLeft  = currentPos.x < previousPos.x;

    bool hitRightSide =
    movingRight &&
    currentRight >= wallPos.x &&
    previousRight < wallPos.x &&
    verticallyOverlapping;

    bool hitLeftSide =
    movingLeft &&
    currentLeft <= wallPos.x + wallSize.x &&
    previousLeft > wallPos.x + wallSize.x &&
    verticallyOverlapping;

    if (hitLeftSide) std::cout << "Left wall hit!\n";
    if (hitRightSide) std::cout << "Right wall hit!\n";
    return hitLeftSide || hitRightSide;
}

bool Collision:: CheckAABBCollision(const AABB& a, const AABB& b) {
    return std::abs(a.pos.x - b.pos.x) <= (a.halfSize.x + b.halfSize.x) &&
           std::abs(a.pos.y - b.pos.y) <= (a.halfSize.y + b.halfSize.y);
}