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
GameObject* instantiateGameObject() { return new T; }

std::map<std::string, GameObject *(*)()> g_GameObjectInstatiators;

void Game::initGameObjectInstatiators() {
    g_GameObjectInstatiators["Ant"] = &instantiateGameObject<Ant>;
    g_GameObjectInstatiators["AntLine"] = &instantiateGameObject<AntLine>;
    g_GameObjectInstatiators["AntNest"] = &instantiateGameObject<AntNest>;
    g_GameObjectInstatiators["ComputerPlayer"] = &instantiateGameObject<ComputerPlayer>;
    g_GameObjectInstatiators["Player"] = &instantiateGameObject<Player>;
    g_GameObjectInstatiators["Wall"] = &instantiateGameObject<Wall>;
}

Game::Game(const std::filesystem::path& path) {
    PN_LOG("loading game from " << path);

    std::ifstream saveFile;
    saveFile.open(path, std::fstream::in);

    if (saveFile.fail())
        throw SaveException();

    std::getline(saveFile, m_mapName);  // skip save name line
    std::getline(saveFile, m_mapName);

    if (saveFile.fail())
        throw SaveException();

    std::string line;
    while (!std::getline(saveFile, line).fail()) {
        std::stringstream lineStream(line);

        std::string objectName;
        lineStream >> objectName;

        PN_LOG("loading " << objectName << " from \"" << line << '"');

        auto instantiator = g_GameObjectInstatiators.find(objectName);
        if (instantiator == g_GameObjectInstatiators.end())
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
    saveFile << m_mapName << "\n";

    for (const auto& object : m_objects) {
        if (!object->serialize(saveFile))
            throw SaveException();
        saveFile << "\n";
    }

    if (saveFile.fail())
        throw SaveException();

    PN_LOG("saved game");
}
