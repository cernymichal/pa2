#include "Game.h"

#include <iostream>

#include "GameSaving.cpp"
#include "log.h"

void Game::addObject(GameObject* object) {
    auto iter = _objects.begin();
    for (; iter != _objects.end() && (*iter)->updatePriority < object->updatePriority; iter++)
        ;

    _objects.emplace(iter, object);
}

void Game::update() {
    for (std::unique_ptr<GameObject>& object : _objects)
        object->update();

    _draw();
}

void Game::_draw() {
    clear();

    for (std::unique_ptr<GameObject>& object : _objects)
        object->draw();

    refresh();
}

std::ostream& Game::log(std::ostream& stream) const {
    stream << "objects: {" << std::endl;

    for (const std::unique_ptr<GameObject>& object : _objects)
        object->log(stream) << "," << std::endl;

    return stream << "}";
}
