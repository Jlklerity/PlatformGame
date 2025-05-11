 
#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Texture.h"
#include "Collision.h"
#include "Wall.h"
#include <iostream>

 
class Orb {
    unsigned int VAO, VBO, EBO;
    AABB orbAABB;
public:
    glm::vec2 position;
    glm::vec2 velocity;
    glm::vec2 size;
    float gravity;  // Add gravity factor

    Orb(glm::vec2 pos, glm::vec2 vel, glm::vec2 sz, float gravity);
     
    void Update(float dt);
    void Draw(const Shader& shader, const Texture& texture,  float aspectRatio, float scale, float angle);

    void BounceIfCollidingWithCannon(const glm::vec2& platformPos, const glm::vec2& platformSize);
    void BounceIfCollidingWithWall(const AABB& wallLeft, const AABB& wallRight, const AABB& wallTop);
    void ResetOrb();
    ~Orb();

};

