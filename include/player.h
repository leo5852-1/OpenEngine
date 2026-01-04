#pragma once
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

using std::vector;

class Player {
public:
    Player();
    Player(float x, float y);
    void setPos(float, float);
    void update(float dt);

    float x;
    float y;
    float mvSpeed = 200.0f;
    float angle = 0.0f;
private:
    float ySpeed = 0.0f; // current y velocity
    float ga = 980.0f; // gravitational acceleration
    bool isGrounded = true;
};
