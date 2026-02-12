#ifndef BOXCOLLIDER_HPP
#define BOXCOLLIDER_HPP

#include "collider.h"

class BoxCollider : public Collider {
public:
    glm::vec3 size; // length, width, height info of box
    BoxCollider(glm::vec3 s) : Collider(ColliderType::Box), size(s) {} 

    // returns collision box in AABB struct form
    // pos: center of object
    AABB getWorldAABB(glm::vec3 pos){
        glm::vec3 center = pos + this->offset; 
        return {(center - size / 2.0f), (center + size / 2.0f)};
    }
};

#endif