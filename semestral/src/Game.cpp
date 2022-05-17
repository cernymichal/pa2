#include "Game.h"

#include "log.h"

Game::Game() {
}

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

bool Game::save(const std::filesystem::path& path) const {
    PN_LOG("saving game to " << path);
    // TODO save game

    return false;
}

Game Game::load(const std::filesystem::path& path) {
    PN_LOG("loading game from " << path);
    // TODO load game

    return Game();
}

std::ostream& Game::log(std::ostream& stream) const {
    stream << "objects: {" << std::endl;

    for (const std::unique_ptr<GameObject>& object : _objects)
        object->log(stream) << "," << std::endl;

    return stream << "}";
}
