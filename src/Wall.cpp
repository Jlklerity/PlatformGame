#include "Wall.h"
#include <glad/glad.h>

static float vertices[] = {
    // positions        // colors        // tex coords

    // Left wall (from -1.0 to -0.95 in X)
    -1.0f, -1.0f,       1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
    -0.95f, -1.0f,      1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
    -0.95f,  1.0f,      1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
    -1.0f,  1.0f,       1.0f, 1.0f, 1.0f,   0.0f, 1.0f,

    // Right wall (from +0.95 to +1.0 in X)
     0.95f, -1.0f,      1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
     1.0f, -1.0f,       1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
     1.0f,  1.0f,       1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
     0.95f,  1.0f,      1.0f, 1.0f, 1.0f,   0.0f, 1.0f,

    // Top wall (from +0.95 to +1.0 in Y)
    -1.0f,  0.95f,      1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
     1.0f,  0.95f,      1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
     1.0f,  1.0f,       1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
    -1.0f,  1.0f,       1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
};

static unsigned int indices[] = {
    // Left wall (0–3)
    0, 1, 2,
    2, 3, 0,

    // Right wall (4–7)
    4, 5, 6,
    6, 7, 4,

    // Top wall (8–11)
    8, 9,10,
    10,11, 8
};
 

Wall::Wall(glm::vec2 pos, glm::vec2 sz)
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

void Wall::Draw(const Shader& shader, const Texture& texture, float aspectRatio) {
    float transform[] = {
        size.x, 0.0f,      0.0f, 0.0f,
        0.0f, size.y,          0.0f, 0.0f,
        0.0f, 0.0f,            1.0f, 0.0f,
        position.x, position.y, 0.0f, 1.0f
    };

    shader.use();
    glUniform1i(glGetUniformLocation(shader.ID, "useTexture"), 0);
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "transform"), 1, GL_FALSE, transform);

    texture.Bind();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
}
