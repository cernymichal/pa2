#include "Game.h"

#include <iostream>

#include "GameSaving.cpp"
#include "log.h"

void Game::addObject(GameObject* object) {
    object->_game = this;

    auto iter = _objects.begin();
    for (; iter != _objects.end() && (*iter)->updatePriority < object->updatePriority; iter++)
        ;

    _objects.emplace(iter, object);
}

void Game::update() {
    for (auto& object : _objects)
        object->update();

    _collision();

    for (auto iterA = _objects.begin(); iterA != _objects.end(); iterA++) {
        if ((*iterA)->dead)
            iterA = _objects.erase(iterA);
    }

    _draw();
}

void Game::_collision() {
    for (auto iterA = _objects.begin(); iterA != _objects.end(); iterA++) {
        auto iterB = iterA;
        for (iterB++; iterB != _objects.end(); iterB++) {
            if ((*iterA)->x == (*iterB)->x && (*iterA)->y == (*iterB)->y) {
                (*iterA)->collideWith(**iterB);
                (*iterB)->collideWith(**iterA);
            }
        }
    }
}

void Game::_draw() {
    clear();

    for (auto& object : _objects)
        object->draw();

    refresh();
}

uint8_t Game::maxPlayers() const {
    uint8_t startingPoints = 0;

    for (const auto& object : _objects) {
        // TODO maxPlayers
    }

    return 5;  // startingPoints;
}

std::ostream& Game::log(std::ostream& stream) const {
    stream << "objects: {" << std::endl;

    for (const auto& object : _objects)
        object->log(stream) << "," << std::endl;

    return stream << "}";
}
