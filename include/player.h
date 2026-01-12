#pragma once
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

using std::vector;

class Player {
public:
    Player();
    Player(glm::vec3);

    void setPos(glm::vec3);
    void update(float dt);
    void jump();
    
    glm::vec3 cameraPos = glm::vec3(0.0f, 1.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    float moveSpeed = 5.0f;
    float rotateSpeed = 2.0f;

private:
    float verticalVelocity = 0.0f; 
    float gravity = 9.8f;          
    float groundLevel = 0.0f;      
    bool isGrounded = false;       
};
