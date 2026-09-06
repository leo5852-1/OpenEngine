#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <glm/glm.hpp>
#include "collider.h"

class GameObject {
public:
    static constexpr float GRAVITY = 9.8f;

    glm::vec3 position;
    Collider* collider;
    bool isStatic;

    bool useGravity = false;               // true면 매 프레임 중력이 velocity에 누적된다
    glm::vec3 velocity = glm::vec3(0.0f);
    bool isGrounded = false;               // 직전 프레임에 바닥(위쪽 방향 mtv)과 충돌했는지

    GameObject() : position(0.0f), collider(nullptr), isStatic(false) {};

    virtual ~GameObject() {
        if(collider)
            delete collider;
    }

    // set the collider of this GameObject
    void setCollider(Collider* c) {
        if(collider)
            delete collider;
        this->collider = c;
    }

    // 중력을 velocity에 적용하고 velocity만큼 이동시킨다.
    // isGrounded는 여기서 일단 false로 리셋되고, 이번 프레임에 바닥과 충돌하면 onCollision에서 다시 true가 된다.
    virtual void update(float dt) {
        isGrounded = false;

        if (!isStatic && useGravity) {
            velocity.y -= GRAVITY * dt;
            position += velocity * dt;
        }
    }

    virtual void onCollision(glm::vec3 mtv) {
        position += mtv;

        if (useGravity) {
            if (mtv.y > 0.0f) {
                velocity.y = 0.0f;
                isGrounded = true;
            } else if (mtv.y < 0.0f) {
                velocity.y = 0.0f;
            }
        }
    }
};


#endif 