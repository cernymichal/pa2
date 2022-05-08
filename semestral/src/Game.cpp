#include "Game.h"

#include <algorithm>

void Game::addObject(GameObject *object) {
    auto compareGameObjects = [](const GameObject *a, const std::unique_ptr<GameObject> &b) {
        return a->updatePriority < b->updatePriority;
    };

    _objects.emplace(
        std::upper_bound(_objects.begin(), _objects.end(), object, compareGameObjects),
        static_cast<GameObject *>(object));
}

void Game::update() {
    for (std::unique_ptr<GameObject> &object : _objects)
        object->update();
}

void Game::draw() {
    for (std::unique_ptr<GameObject> &object : _objects)
        object->draw();

    refresh();
}

bool Game::save(const char *fileName) const {
    // todo

    return false;
}

Game Game::load(const char *fileName) {
    // todo

    return Game();
}
