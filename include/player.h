#pragma once
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

#include "gameObject.h"
#include "boxCollider.h"

using std::vector;

class Player: public GameObject {
public:
    Player();
    Player(glm::vec3);

    void setPos(glm::vec3);
    void jump();
    void translate(glm::vec3 vec);

    glm::vec3 cameraOffset = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    float moveSpeed = 5.0f;
    float rotateSpeed = 2.0f;
    float jumpSpeed = 4.0f;
};
