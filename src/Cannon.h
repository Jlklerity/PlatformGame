#ifndef PLATFORM_H
#define PLATFORM_H

#include <glm/glm.hpp>
#include "Shader.h"
#include "Texture.h"
#include <iostream>

class Cannon {
    unsigned int VAO, VBO, EBO;
public:
    glm::vec2 position;
    glm::vec2 size;

    Cannon(glm::vec2 pos, glm::vec2 sz);
    void Update(const glm::vec2& pos);
    void Draw(const Shader& shader, const Texture& texture, float aspectRatio);
    void ResetCannon();
    ~Cannon();
};

#endif
