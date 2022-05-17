#include "Game.h"

#include "log.h"

Game::Game() {
}

void Game::addObject(GameObject *object) {
    auto iter = _objects.begin();
    for (; iter != _objects.end() && (*iter)->updatePriority < object->updatePriority; iter++)
        ;

    _objects.emplace(iter, object);
    // TODO static_cast<GameObject *>(object)
}

void Game::update() {
    for (std::unique_ptr<GameObject> &object : _objects)
        object->update();

    _draw();
}

void Game::_draw() {
    clear();

    for (std::unique_ptr<GameObject> &object : _objects)
        object->draw();

    refresh();
}

bool Game::save(const char *fileName) const {
    // TODO save game

    return false;
}

Game Game::load(const char *fileName) {
    // TODO load game

    return Game();
}

std::ostream &Game::log(std::ostream &stream) const {
    stream << "objects: {" << std::endl;

    for (const std::unique_ptr<GameObject> &object : _objects)
        object->log(stream) << "," << std::endl;

    return stream << "}";
}
