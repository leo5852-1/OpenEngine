#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include <memory>
#include <algorithm>
#include <type_traits>
#include <utility>

#include "renderableObject.h"
#include "collisionSystem.h"

// 렌더링되는 월드 오브젝트(Cube, Plane 등)의 소유, 콜리전 등록/해제, draw를 한 곳에서 관리
// spawn()으로 한 번에 생성하도록
class Scene {
public:
    Scene(CollisionSystem& collisionSystem) : collisionSystem(collisionSystem) {}

    // T는 RenderableObject를 상속해야 한다. 생성자 인자를 그대로 전달한다.
    template<typename T, typename... Args>
    T& spawn(Args&&... args) {
        static_assert(std::is_base_of<RenderableObject, T>::value, "Scene::spawn requires a RenderableObject");

        auto obj = std::make_unique<T>(std::forward<Args>(args)...);
        T& ref = *obj;

        if (ref.collider) {
            collisionSystem.registerObject(&ref);
        }

        objects.push_back(std::move(obj));
        return ref;
    }

    // 콜리전 등록 해제 + 소유권 해제(소멸자에서 GL 리소스도 함께 정리됨)
    void destroy(RenderableObject* obj) {
        if (obj->collider) {
            collisionSystem.unregisterObject(obj);
        }

        objects.erase(std::remove_if(objects.begin(), objects.end(),
            [obj](const std::unique_ptr<RenderableObject>& owned) { return owned.get() == obj; }),
            objects.end());
    }

    void update(float dt) {
        for (auto& obj : objects) {
            obj->update(dt);
        }
    }

    void draw() {
        for (auto& obj : objects) {
            obj->draw();
        }
    }

private:
    CollisionSystem& collisionSystem;
    std::vector<std::unique_ptr<RenderableObject>> objects;
};

#endif
