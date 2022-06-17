#pragma once

#include <fstream>
#include <map>

#include "Game.h"
#include "GameObject/Ant.h"
#include "GameObject/AntLine.h"
#include "GameObject/AntNest.h"
#include "GameObject/ComputerPlayer.h"
#include "GameObject/GameObject.h"
#include "GameObject/Player.h"
#include "GameObject/Wall.h"
#include "Save.h"
#include "log.h"

template <typename T>
GameObject* instantiateGO() { return new T; }

std::map<std::string, GameObject* (*)()> GO_NAME_MAP;

void Game::initGONameMap() {
    GO_NAME_MAP["Ant"] = &instantiateGO<Ant>;
    GO_NAME_MAP["AntLine"] = &instantiateGO<AntLine>;
    GO_NAME_MAP["AntNest"] = &instantiateGO<AntNest>;
    GO_NAME_MAP["ComputerPlayer"] = &instantiateGO<ComputerPlayer>;
    GO_NAME_MAP["Player"] = &instantiateGO<Player>;
    GO_NAME_MAP["Wall"] = &instantiateGO<Wall>;
}

Game::Game(const std::filesystem::path& path) {
    PN_LOG("loading game from " << path);

    std::ifstream saveFile;
    saveFile.open(path, std::fstream::in);

    if (saveFile.fail())
        throw SaveException();

    std::getline(saveFile, mapName);  // skip save name line
    std::getline(saveFile, mapName);

    if (saveFile.fail())
        throw SaveException();

    std::string line;
    while (!std::getline(saveFile, line).fail()) {
        std::stringstream lineStream(line);

        std::string objectName;
        lineStream >> objectName;

        PN_LOG("loading " << objectName << " from \"" << line << '"');

        auto instantiator = GO_NAME_MAP.find(objectName);
        if (instantiator == GO_NAME_MAP.end())
            throw SaveException();

        auto object = (*instantiator->second)();
        if (!object->unserialize(lineStream))
            throw SaveException();

        addObject(object);
    }

    PN_LOG("loaded game");
}

void Game::save(const std::filesystem::path& path) const {
    PN_LOG("saving game to " << path);

    try {
        std::filesystem::create_directories(path.parent_path());
    }
    catch (std::filesystem::filesystem_error&) {
        throw SaveException();
    }

    std::ofstream saveFile;
    saveFile.open(path, std::fstream::out);

    saveFile << path.stem().string() << "\n";
    saveFile << mapName << "\n";

    for (const auto& object : _objects) {
        if (!object->serialize(saveFile))
            throw SaveException();
        saveFile << "\n";
    }

    if (saveFile.fail())
        throw SaveException();

    PN_LOG("saved game");
}
