#include "player.h"

Player::Player() {
    isStatic = false;
    setCollider(new BoxCollider(glm::vec3(0.6f, 1.0f, 0.6f)));
    collider->offset = glm::vec3(0.0f, 0.5f, 0.0f);
    position = glm::vec3(0.0f, 1.0f, 3.0f);
}  

Player::Player(glm::vec3 pos){
    setCollider(new BoxCollider(glm::vec3(0.6f, 1.0f, 0.6f)));
    setPos(pos);
}

void Player::setPos(glm::vec3 pos){
    position = pos;
}
 
void Player::update(float dt){ 
    if (!isGrounded) {
        verticalVelocity -= this->gravity * dt;
        position.y += verticalVelocity * dt;
    }

}

void Player::onCollision(glm::vec3 mtv) {
    position += mtv;

    if (mtv.y > 0.0f) {
        // 위로 밀렸다 = 바닥에 착지
        verticalVelocity = 0.0f;
        isGrounded = true;
    }
    if (mtv.y < 0.0f) {
        // 아래로 밀렸다 = 천장에 머리를 박음
        verticalVelocity = 0.0f;
    }
}

void Player::jump() {
    if (this->isGrounded) {
        this->verticalVelocity = 4.0f;
        this->isGrounded = false;
    }
}