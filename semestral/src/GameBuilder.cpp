#include "GameBuilder.h"

#include <algorithm>
#include <fstream>
#include <random>

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

std::map<std::string, GameObject* (*)()> g_GameObjectInstatiators;

void GameBuilder::initGameObjectInstatiators() {
    g_GameObjectInstatiators["Ant"] = &instantiateGameObject<Ant>;
    g_GameObjectInstatiators["AntLine"] = &instantiateGameObject<AntLine>;
    g_GameObjectInstatiators["AntNest"] = &instantiateGameObject<AntNest>;
    g_GameObjectInstatiators["ComputerPlayer"] = &instantiateGameObject<ComputerPlayer>;
    g_GameObjectInstatiators["Player"] = &instantiateGameObject<Player>;
    g_GameObjectInstatiators["Wall"] = &instantiateGameObject<Wall>;
}

GameBuilder::GameBuilder() {
}

std::unique_ptr<Game>&& GameBuilder::getGame() {
    return std::move(m_game);
}

void GameBuilder::createPlayers(uint8_t aiPlayers) {
    m_game->addObject<Player>(0, COLOR_PAIR_BLUE, "Player");

    for (uint8_t i = 1; i <= aiPlayers; i++)
        m_game->addObject<ComputerPlayer>(i, COLOR_PAIR_BLUE + i, std::string("AI").append(std::to_string(i)));

    PN_LOG("created " << static_cast<unsigned short>(aiPlayers) << "ai players");

    // randomly choose player starting nests
    std::vector<AntNest*> startingNests;
    for (const auto& nest : m_game->m_nestMap) {
        if (nest.second->starting())
            startingNests.push_back(nest.second);
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(startingNests.begin(), startingNests.end(), g);

    auto nestIter = startingNests.begin();
    auto playerIter = m_game->m_playerMap.begin();
    for (; playerIter != m_game->m_playerMap.end(); playerIter++, nestIter++)
        (*nestIter)->changeOwningPlayer(playerIter->second);
}

uint8_t GameBuilder::maxPlayers() const {
    uint8_t startingPoints = 0;

    for (const auto& nest : m_game->m_nestMap) {
        if (nest.second->starting())
            startingPoints++;
    }

    return startingPoints;
}

void GameBuilder::loadFromFile(const std::filesystem::path& path) {
    m_game = std::unique_ptr<Game>(new Game());

    PN_LOG("loading game from " << path);

    std::ifstream saveFile;
    saveFile.open(path, std::fstream::in);

    if (saveFile.fail())
        throw SaveException();

    std::getline(saveFile, m_game->m_mapName);  // skip save name line
    std::getline(saveFile, m_game->m_mapName);

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

        m_game->addObject(object);
    }

    PN_LOG("loaded game");
}

void GameBuilder::saveToFile(const Game& game, const std::filesystem::path& path) {
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
    saveFile << game.m_mapName << "\n";

    for (const auto& object : game.m_objects) {
        if (!object->serialize(saveFile))
            throw SaveException();
        saveFile << "\n";
    }

    if (saveFile.fail())
        throw SaveException();

    PN_LOG("saved game");
}
