#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include "Cannon.h"
#include "Wall.h"
#include "TextRenderer.h"
#include "orb.h"

// Globals
float lastFrameTime = 0.0f;
float posX = 0.0f, posY = -0.6f, speed = 2.0f;
int frameCount = 0;
float lastFPSUpdateTime = 0.0f;
int avgFPS = 0;

// Global pointers
Orb* orb = nullptr;
Cannon* cannon = nullptr;
TextRenderer* text = nullptr;
Wall* wall = nullptr;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}


void processInput(GLFWwindow* window, float& x, float& y, float speed, float deltaTime){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        y += speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        y -= speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        x -= speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        x += speed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        
        orb->ResetOrb();
        cannon->ResetCannon(); 
    }
}


int main(){
    int WIDTH = 800;
    int HEIGHT = 600;
    const char* TITLE = "Cannon Destroyer";

    if (!glfwInit())
        return -1;
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }  
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    

    Shader shader("../shaders/vertex_shader.txt", "../shaders/fragment_shader.txt");
    Texture texture("../include/Images/clipart2634359.png");
    float aspectRatio = texture.GetAspectRatio();

    // Text rendering (FPS)
    text = new TextRenderer(WIDTH, HEIGHT);
    text->Load("../fonts/arial/ARIAL.TTF", 24);

    // Create orb
    glm::vec2 orbStartPos(0.0f, -0.8f);
    glm::vec2 orbVel(0.3f, 0.9f);
    glm::vec2 orbSize(0.1f, 0.1f);
    float gravity = 0.1f;
    orb = new Orb(orbStartPos, orbVel, orbSize, gravity);
    //Create cannon
    glm::vec2 CannonPos(0.0f, -0.9f);  // Position (near the bottom)
    glm::vec2 CannonSize(0.5f, 0.1f);  // Size (width = 1.0, height = 0.1)
    cannon = new Cannon(CannonPos, CannonSize);

    // Create left wall
    glm::vec2 wallPos(0.0f, 0.0f);  // Position (near the bottom)
    glm::vec2 wallSize(1.0f, 1.0f);  // Size (width = 1.0, height = 0.1)

    Wall* wall = new Wall(wallPos, wallSize);
    AABB wallLeft  = { glm::vec2(-0.975f, 0.0f), glm::vec2(0.025f, 1.0f) };
    AABB wallRight = { glm::vec2( 0.975f, 0.0f), glm::vec2(0.025f, 1.0f) };
    AABB wallTop   = { glm::vec2( 0.0f,  0.975f), glm::vec2(1.0f, 0.025f) };
    

    //Render Loop
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrameTime;
        lastFrameTime = currentFrame;
        frameCount++;

        // Update FPS every second
        if (currentFrame - lastFPSUpdateTime >= 1.0f) {
            avgFPS = frameCount;
            frameCount = 0;
            lastFPSUpdateTime = currentFrame;
        }
        
        
        processInput(window, posX, posY, 1.0f, deltaTime);
        //Render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //Draw Walls
        wall->Draw(shader, texture, aspectRatio);
         

         // Draw orb
        orb->Update(deltaTime);
        orb->Draw(shader, texture, aspectRatio, 1.0f, currentFrame * 1.0f);
        orb->BounceIfCollidingWithCannon(glm::vec2(posX, -0.9f), CannonSize);
        orb->BounceIfCollidingWithWall(wallLeft, wallRight, wallTop);
         

        cannon->Update(glm::vec2(posX, -0.9f));
        cannon->Draw(shader, texture, aspectRatio);
        // Draw FPS
        text->RenderText("FPS: " + std::to_string(avgFPS), 30.0f, HEIGHT - 40.0f, 1.0f, glm::vec3(1.0f,1.0f,1.0f));

         
         
         

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    delete orb;
    delete text;
    delete cannon;
    delete wall;
    // delete wallLeft;
    // delete wallRight;
    std::cout<<"Window closed!"<<"\n";
    return 0;
}   
