#include "player.h"

Player::Player() {
}

Player::Player(glm::vec3 pos){
    setPos(pos);
}

void Player::setPos(glm::vec3 pos){
    this->cameraPos = pos;
}

void Player::update(float dt){
    this->verticalVelocity -= this->gravity * dt;
    this->cameraPos.y += this->verticalVelocity * dt;
    float groundLimit = 1.0f;

    if (this->cameraPos.y <= groundLimit) {
        this->cameraPos.y = groundLimit; 
        this->verticalVelocity = 0.0f;   
        this->isGrounded = true;
    } 
    else
        this->isGrounded = false;
}

void Player::jump() {
    if (this->isGrounded) {
        this->verticalVelocity = 5.0f;
        this->isGrounded = false;
    }
}