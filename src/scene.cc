#include "scene.hh"

void Scene::update(double deltaTime) {
    static double time = 0;
    time += deltaTime;
    for (auto& object : _pObjects) {
        object->move(time);
    }
}