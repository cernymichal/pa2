#include "Game.h"

#include <algorithm>
#include <iostream>

Game::Game() {
    timeoutDelay = -1;  // hello world screen only!!
    // timeoutDelay = 0;  // non blocking input
}

void Game::addObject(GameObject *object) {
    auto compareGameObjects = [](const GameObject *a, const std::unique_ptr<GameObject> &b) {
        return a->updatePriority < b->updatePriority;
    };

    _objects.emplace(
        std::upper_bound(_objects.begin(), _objects.end(), object, compareGameObjects),
        static_cast<GameObject *>(object));
}

void Game::update(double dt, int key) {
    // todo

    for (std::unique_ptr<GameObject> &object : _objects)
        object->update();

    draw();
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

std::ostream &Game::log(std::ostream &stream) const {
    stream << "objects: {" << std::endl;

    for (const std::unique_ptr<GameObject> &object : _objects)
        object->log(stream) << "," << std::endl;

    return stream << "}";
}
