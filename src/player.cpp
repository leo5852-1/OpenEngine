#include "player.h"

Player::Player(){
    setPos(300.0f, 300.0f);
}
Player::Player(float x, float y){
    setPos(x, y);
}

void Player::setPos(float x, float y){
    this->x = x;
    this->y = y;
}

void Player::update(float dt) {
    this->ySpeed += ga * dt;
    y -= ySpeed * dt;

    // if y off limit
    if (y <= 0.0f) {
        y = 0.0f;          // relocate to y=0(floor)
        ySpeed = 0.0f;  // reset ySpeed
        isGrounded = true;
    } else {
        isGrounded = false;
    }
}
