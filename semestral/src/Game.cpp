#include "Game.h"

#include <fstream>
#include <iostream>

#include "log.h"

Game::Game(const std::filesystem::path& path) {
    PN_LOG("loading game from " << path);

    // TODO check for fs errors

    std::ifstream saveFile;
    saveFile.open(path, std::fstream::in);

    std::getline(saveFile, mapName);  // skip save name line
    std::getline(saveFile, mapName);

    // TODO load game

    PN_LOG("loaded game");
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

    // TODO check for fs errors

    std::filesystem::create_directories(path.parent_path());

    std::ofstream saveFile;
    saveFile.open(path, std::fstream::out);

    saveFile << path.stem().string() << "\n";
    saveFile << mapName << "\n";

    // TODO save game

    PN_LOG("saved game");

    return false;
}

std::ostream& Game::log(std::ostream& stream) const {
    stream << "objects: {" << std::endl;

    for (const std::unique_ptr<GameObject>& object : _objects)
        object->log(stream) << "," << std::endl;

    return stream << "}";
}
