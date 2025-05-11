#include "Cannon.h"
#include <glad/glad.h>


static float vertices[] = {
    // Cannon Base (wide, short rectangle)
    // x,     y,       r,    g,    b,      u,    v
    -0.3f, -0.05f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,  // Bottom-left
     0.3f, -0.05f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,  // Bottom-right
     0.3f,  0.05f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,  // Top-right
    -0.3f,  0.05f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f,  // Top-left

    // Cannon Barrel (narrow, tall rectangle above the base)
    -0.05f,  0.05f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f,  // Bottom-left
     0.05f,  0.05f,  1.0f, 1.0f, 1.0f,   1.0f, 0.0f,  // Bottom-right
     0.05f,  0.4f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,  // Top-right
    -0.05f,  0.4f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f   // Top-left

};

static int indices[] = {
    // Base
    0, 1, 2,
    2, 3, 0,

    // Barrel
    4, 5, 6,
    6, 7, 4
};
Cannon::Cannon(glm::vec2 pos, glm::vec2 sz)
    : position(pos), size(sz) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        // position attribute (location = 0)
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // color attribute (location = 1)
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1); 
        // texture coord attribute (location = 2)
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }
void Cannon::Update(const glm::vec2& pos) {
    position = pos; // always apply input

    // then clamp it inside the allowed range
    if (position.x > 0.9f) {
        position.x = 0.9f;
    }
    else if (position.x < -0.9f) {
        position.x = -0.9f;
    }
}

void Cannon::Draw(const Shader& shader, const Texture& texture, float aspectRatio) {
    float transform[] = {
        size.x *aspectRatio, 0.0f, 0.0f, 0.0f,
        0.0f, size.y,          0.0f, 0.0f,
        0.0f, 0.0f,            1.0f, 0.0f,
        position.x, position.y, 0.0f, 1.0f
    };
    
    shader.use();
    
    glUniform1i(glGetUniformLocation(shader.ID, "useTexture"), false);
    texture.Bind();
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "transform"), 1, GL_FALSE, transform);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
}
void Cannon::ResetCannon() {
    this->position = glm::vec2(0.0f, -0.9f);  // Reset to initial position
     std::cout<<"Cannon reset!"<<"\n";
}
Cannon::~Cannon() {
    glDeleteVertexArrays(1, &VAO);  
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}