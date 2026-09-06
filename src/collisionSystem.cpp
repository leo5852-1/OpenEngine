#include "collisionSystem.h"
#include <algorithm>
#include <iostream>

void CollisionSystem::registerObject(GameObject* obj){
    if (!obj->collider) {
        std::cout << "CollisionSystem::registerObject - collider가 없는 오브젝트는 등록할 수 없습니다.\n";
        return;
    }
    this->objects.push_back(obj);
}

void CollisionSystem::unregisterObject(GameObject* obj){
    this->objects.erase(std::remove(this->objects.begin(), this->objects.end(), obj), this->objects.end());
}

void CollisionSystem::update(){
    for (size_t i = 0; i < objects.size(); i++) {
        for (size_t j = i + 1; j < objects.size(); j++) {
            GameObject* a = objects[i];
            GameObject* b = objects[j];

            // static끼리는 검사 불필요
            if (a->isStatic && b->isStatic) continue;

            // a를 b로부터 밀어내는 MTV 계산
            auto mtv = getMTV(*a, *b);
            if (!mtv) continue;

            if (!a->isStatic && !b->isStatic) {
                // 둘 다 dynamic: 절반씩 나눠서 밀어냄
                a->onCollision( (*mtv) / 2.0f);
                b->onCollision(-(*mtv) / 2.0f);
            } else if (!a->isStatic) {
                a->onCollision( (*mtv));
            } else {
                b->onCollision(-(*mtv));
            }
        }
    }
}

std::optional<glm::vec3> CollisionSystem::getMTV(GameObject& a, GameObject& b) {
    // Box vs Box Collision detection
    if (a.collider->type == ColliderType::Box && b.collider->type == ColliderType::Box) {
        auto* boxA = static_cast<BoxCollider*>(a.collider);
        auto* boxB = static_cast<BoxCollider*>(b.collider);
        return getBoxVSBoxMTV(*boxA, a.position, *boxB, b.position);
    }
    return std::nullopt;
}

std::optional<glm::vec3> CollisionSystem::getBoxVSBoxMTV(BoxCollider& a, glm::vec3 posA, BoxCollider& b, glm::vec3 posB) {
    AABB aabb_a = a.getWorldAABB(posA);
    AABB aabb_b = b.getWorldAABB(posB);

    if (!AABB::checkCollision(aabb_a, aabb_b)) return std::nullopt;

    // calculate overlapping for each axis
    // since collsion is confirmed, the values are all non-negative
    float ox = std::min(aabb_a.max.x, aabb_b.max.x) - std::max(aabb_a.min.x, aabb_b.min.x);
    float oy = std::min(aabb_a.max.y, aabb_b.max.y) - std::max(aabb_a.min.y, aabb_b.min.y);
    float oz = std::min(aabb_a.max.z, aabb_b.max.z) - std::max(aabb_a.min.z, aabb_b.min.z);

    // b -> a direction
    glm::vec3 centerA = posA + a.offset;
    glm::vec3 centerB = posB + b.offset;
    glm::vec3 dir = centerA - centerB;

    // return the MTV
    if (ox <= oy && ox <= oz)
        return glm::vec3(dir.x > 0 ? ox : -ox, 0.0f, 0.0f);
    else if (oy <= ox && oy <= oz)
        return glm::vec3(0.0f, dir.y > 0 ? oy : -oy, 0.0f);
    else
        return glm::vec3(0.0f, 0.0f, dir.z > 0 ? oz : -oz);
}