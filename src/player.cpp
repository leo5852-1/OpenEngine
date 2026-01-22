#include "player.h"

Player::Player() {
    ;
}

Player::Player(glm::vec3 pos){
    setPos(pos);
}

void Player::setPos(glm::vec3 pos){
    this->cameraPos = pos;
}

void Player::update(float dt, const std::vector<Plane>& worldObjects){
    this->verticalVelocity -= this->gravity * dt;
    float nextY = this->cameraPos.y + this->verticalVelocity * dt;
    float groundLimit = 1.0f;

    AABB playerBox;
    playerBox.min = glm::vec3(cameraPos.x - 0.5f, nextY - 1.0f, cameraPos.z - 0.5f);
    playerBox.max = glm::vec3(cameraPos.x + 0.5f, nextY, cameraPos.z + 0.5f);

    bool hitGround = false;

    for(const auto& obj : worldObjects) {
        //check if obj's AABB collides with player's AABB
        AABB planeBox = const_cast<Plane&>(obj).getAABB();
        // if collision 
        if (AABB::checkCollision(playerBox, planeBox)) {
            // land on the ground
            if (this->verticalVelocity < 0) {
                this->verticalVelocity = 0.0f;
                this->isGrounded = true;
                
                // prevent from passing through
                this->cameraPos.y = planeBox.max.y + 1.0f; 
                hitGround = true;
            }
            break;
        }
    }
    if (!hitGround) {
        // if no collsion: set nextY as the Y value
        this->cameraPos.y = nextY;
        this->isGrounded = false;
    }
}

void Player::jump() {
    if (this->isGrounded) {
        this->verticalVelocity = 4.0f;
        this->isGrounded = false;
    }
}