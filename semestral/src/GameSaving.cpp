#include <fstream>
#include <map>

#include "Game.h"
#include "GameObject/Ant.h"
#include "GameObject/AntNest.h"
#include "GameObject/ComputerPlayer.h"
#include "GameObject/GameObject.h"
#include "GameObject/Player.h"
#include "GameObject/Wall.h"
#include "log.h"

template <typename T>
GameObject* instantiateGO() { return new T; }

std::map<std::string, GameObject* (*)()> GO_NAME_MAP;

void Game::initGONameMap() {
    GO_NAME_MAP["Ant"] = &instantiateGO<Ant>;
    GO_NAME_MAP["AntNest"] = &instantiateGO<AntNest>;
    GO_NAME_MAP["ComputerPlayer"] = &instantiateGO<ComputerPlayer>;
    GO_NAME_MAP["Player"] = &instantiateGO<Player>;
    GO_NAME_MAP["Wall"] = &instantiateGO<Wall>;
}

Game::Game(const std::filesystem::path& path) {
    PN_LOG("loading game from " << path);

    // TODO check for fs errors

    std::ifstream saveFile;
    saveFile.open(path, std::fstream::in);

    std::getline(saveFile, mapName);  // skip save name line
    std::getline(saveFile, mapName);

    std::string line;
    while (!std::getline(saveFile, line).fail()) {
        std::stringstream lineStream(line);

        std::string objectName;
        lineStream >> objectName;

        PN_LOG("loading " << objectName << " from \"" << line << '"');

        auto instantiator = GO_NAME_MAP.find(objectName);
        if (instantiator == GO_NAME_MAP.end())
            break;  // TODO error

        auto object = (*instantiator->second)();
        object->unserialize(lineStream);  // TODO error

        addObject(object);
    }

    PN_LOG("loaded game");
}

bool Game::save(const std::filesystem::path& path) const {
    PN_LOG("saving game to " << path);

    // TODO check for fs errors

    std::filesystem::create_directories(path.parent_path());

    std::ofstream saveFile;
    saveFile.open(path, std::fstream::out);

    saveFile << path.stem().string() << "\n";
    saveFile << mapName << "\n";

    for (const auto& object : _objects) {
        object->serialize(saveFile);  // TODO error
        saveFile << "\n";
    }

    PN_LOG("saved game");

    return false;
}
