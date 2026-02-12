#ifndef COLLISIONSYSTEM_HPP
#define COLLISIONSYSTEM_HPP

#include "gameObject.h"

class CollisionSystem {
public:
    static bool CheckCollsion(GameObject& a, GameObject& b);
    static void ResolveCollsion(GameObject& a, GameObject& b);

private:
    static bool CheckBoxVSBox(GameObject& a, GameObject& b);
    static bool CheckBoxVSSphere(GameObject& a, GameObject& b);
};

#endif