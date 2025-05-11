#pragma once

#include <glm/glm.hpp>
#include "Shader.h"
#include "Texture.h"
#include "Collision.h"



class Wall {
    unsigned int VAO, VBO, EBO;
public:
    glm::vec2 position;
    glm::vec2 size;

    Wall(glm::vec2 pos, glm::vec2 sz);

    void Draw(const Shader& shader, const Texture& texture, float aspectRatio);
};


