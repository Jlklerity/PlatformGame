#include "orb.h"
 
static float vertices[] = {
    // positions    // colors       // tex coords
    -0.2f, -0.2f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,  // bottom left
     0.2f, -0.2f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,  // bottom right
     0.2f,  0.2f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,  // top right
    -0.2f,  0.2f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f   // top left
};

static unsigned int indices[] = {
    0, 1, 2,   // first triangle
    2, 3, 0    // second triangle
};

Orb::Orb(glm::vec2 pos, glm::vec2 vel, glm::vec2 sz, float grav)
    : position(pos), velocity(vel), size(sz), gravity(grav) {
        std::cout<<sz.x<<","<<sz.y<<"\n";
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        // position attribute (location = 0)x`
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // color attribute (location = 1)
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);   
        // texture coord attribute (location = 2)
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }

 
void Orb::Update(float dt) {
    velocity.y -= gravity * dt;
    position += velocity * dt;
    // Keep orbAABB updated
    orbAABB.pos = position;
    orbAABB.halfSize = size * 0.5f;
    
}

void Orb::BounceIfCollidingWithCannon(const glm::vec2& platformPos, const glm::vec2& platformSize) {

    // Bottom of orb and top of platform
     
    float previousBottom = position.y - velocity.y * 0.016f; // assuming ~60fps
    float platformTop = platformPos.y + platformSize.y;

    bool horizontallyOverlapping =
        position.x + size.x > platformPos.x &&
        position.x < platformPos.x + platformSize.x;

    bool isFalling = velocity.y < 0;

    if (horizontallyOverlapping &&
       Collision::IsCollidingVertically(position, size, platformPos, platformSize) &&
        isFalling)
    {
        // Bounce
        velocity.y *= -1.0f;
        std::cout<<"Orb collided with cannon!"<<"\n";
        // Clamp position to top of platform
        position.y = platformTop;

        // Optional: stop small bounces
        if (fabs(velocity.y) < 0.89f) {
            velocity.y = 0.0f;
        }
    }
}
void Orb::BounceIfCollidingWithWall(const AABB& wallLeft, const AABB& wallRight, const AABB& wallTop) {
     
     
    if (Collision::CheckAABBCollision(orbAABB, wallLeft)) {
        velocity.x *= -1;
        position.x = wallLeft.pos.x + wallLeft.halfSize.x + orbAABB.halfSize.x + 0.001f;
    }
    if (Collision::CheckAABBCollision(orbAABB, wallRight)) {
        velocity.x *= -1;
        position.x = wallRight.pos.x - wallRight.halfSize.x - orbAABB.halfSize.x - 0.001f;
    }
    if (Collision::CheckAABBCollision(orbAABB, wallTop)) {
        velocity.y *= -1;
        position.y = wallTop.pos.y - wallTop.halfSize.y - orbAABB.halfSize.y - 0.001f;
    }
}
void Orb::Draw(const Shader& shader, const Texture& texture, float aspectRatio, float scale, float angle) {
    float cosA = cos(angle);
    float sinA = sin(angle);
    float sx = size.x * aspectRatio;
    float sy = size.y;

    float transform[] = {
        cosA * sx,  sinA * sy, 0.0f, 0.0f,
        -sinA * sx,  cosA * sy, 0.0f, 0.0f,
        0.0f,       0.0f,      1.0f, 0.0f,
        position.x / aspectRatio, position.y, 0.0f, 1.0f
    };

    // Bind to texture unit 0
    shader.use();
    // glUniform1i(glGetUniformLocation(shader.ID, "texture1"), 0);
    glUniform1i(glGetUniformLocation(shader.ID, "useTexture"), false);
    texture.Bind();
    
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "transform"), 1, GL_FALSE, transform);
    
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
     
    
}


// In Orb.cpp
void Orb::ResetOrb() {
    this->position = glm::vec2(0.0f, 0.8f);  // Reset to initial position
    this->velocity = glm::vec2(0.3f, 0.9f); // Reset to initial velocity
    
}

Orb::~Orb() {
    glDeleteVertexArrays(1, &VAO);  
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}