#ifndef COLLIDER_HPP
#define COLLIDER_HPP

#include <glm/glm.hpp>

enum class ColliderType {Box, Sphere};

class Collider {
public:
    ColliderType type;
    glm::vec3 offset; // collider offset from object center position

    Collider(ColliderType t) : type(t), offset(0.0f) {}
    virtual ~Collider() {}

    // for visualize collsion bounds for debugging
    // virtual void DebugDraw() = 0;
};

struct AABB {
    glm::vec3 min;
    glm::vec3 max;

    static bool checkCollision(const AABB& box1, const AABB& box2) {
        return (box1.min.x <= box2.max.x && box1.max.x >= box2.min.x) &&
               (box1.min.y <= box2.max.y && box1.max.y >= box2.min.y) &&
               (box1.min.z <= box2.max.z && box1.max.z >= box2.min.z);
    }
};

#endif 