#ifndef COLLISIONSYSTEM_HPP
#define COLLISIONSYSTEM_HPP

#include <vector>
#include <optional>
#include "glm/glm.hpp"
#include "gameObject.h"
#include "boxCollider.h"

class CollisionSystem {
public:
    void registerObject(GameObject* obj);
    void update();

private:
    std::vector<GameObject*> objects;

    static std::optional<glm::vec3> getMTV(GameObject& a, GameObject& b);
    static std::optional<glm::vec3> getBoxVSBoxMTV(BoxCollider& a, glm::vec3 posA,BoxCollider& b, glm::vec3 posB);
    static std::optional<glm::vec3> getBoxVSSphereMTV(GameObject& a, GameObject& b);
};

#endif