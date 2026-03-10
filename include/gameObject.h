#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <glm/glm.hpp>
#include "collider.h"

class GameObject {
public:
    glm::vec3 position;
    Collider* collider;
    bool isStatic;

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

    virtual void onCollision(glm::vec3 mtv) {
        position += mtv;
    }
};


#endif 