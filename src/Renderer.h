#pragma once
#include <glad/glad.h>

class Renderer{
    unsigned int VAO, VBO, EBO;
    float vertices;
    int indices;
public:
    Renderer();
};