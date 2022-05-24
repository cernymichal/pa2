#include "Game.h"

#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

#include "GameSaving.cpp"
#include "log.h"
#include "utils.cpp"

void Game::addObject(GameObject* object) {
    object->_game = this;

    auto iter = _objects.begin();
    for (; iter != _objects.end() && (*iter)->updatePriority < object->updatePriority; iter++)
        ;

    _objects.emplace(iter, object);

    object->onAdd();
}

void Game::onLoad() {
    for (auto& object : _objects)
        object->onLoad();
}

void Game::createPlayers(uint8_t aiPlayers) {
    addObject(
        new Player(0, COLOR_PAIR_BLUE, "Player"));

    for (uint8_t i = 1; i <= aiPlayers; i++)
        addObject(
            new ComputerPlayer(i, COLOR_PAIR_BLUE + i, std::string("AI").append(std::to_string(i))));

    // randomly choose player starting nests
    std::vector<AntNest*> startingNests;
    for (const auto& nest : nestMap) {
        if (nest.second->starting)
            startingNests.push_back(nest.second);
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(startingNests.begin(), startingNests.end(), g);

    auto nestIter = startingNests.begin();
    auto playerIter = playerMap.begin();
    for (; playerIter != playerMap.end(); playerIter++, nestIter++)
        (*nestIter)->changePlayer(playerIter->second);
}

void Game::update() {
    for (auto& object : _objects)
        object->update();

    _collision();

    for (auto iterA = _objects.begin(); iterA != _objects.end(); iterA++) {
        if ((*iterA)->dead)
            iterA = _objects.erase(iterA);
    }
}

void Game::_collision() {
    for (auto iterA = _objects.begin(); iterA != _objects.end(); iterA++) {
        auto iterB = iterA;
        for (iterB++; iterB != _objects.end(); iterB++) {
            if ((*iterA)->hitDistance < 0 || (*iterB)->hitDistance < 0)
                continue;

            auto d = distance((*iterA)->x, (*iterA)->y, (*iterB)->x, (*iterB)->y);
            if (d <= std::max((*iterA)->hitDistance, (*iterB)->hitDistance)) {
                (*iterA)->collideWith(**iterB);
                (*iterB)->collideWith(**iterA);
            }
        }
    }
}

void Game::draw() {
    for (auto& object : _objects)
        object->draw();
}

uint8_t Game::maxPlayers() const {
    uint8_t startingPoints = 0;

    for (const auto& nest : nestMap) {
        if (nest.second->starting)
            startingPoints++;
    }

    return startingPoints;
}

void Game::disableLinesFrom(uint8_t playerId, char nestId) {
    auto nest = nestMap.find(nestId);

    if (nest == nestMap.end() || !nest->second->player() || nest->second->player()->id != playerId)
        return;

    nest->second->disableLines();
}

void Game::activateLine(uint8_t playerId, char nestAId, char nestBId) {
    auto nest = nestMap.find(nestAId);

    if (nest == nestMap.end() || !nest->second->player() || nest->second->player()->id != playerId)
        return;

    auto line = nest->second->lineMap.find(nestBId);

    if (line == nest->second->lineMap.end())
        return;

    nest->second->disableLines();
    line->second->switchSide(nest->second, true);
}

std::ostream& Game::log(std::ostream& stream) const {
    stream << "objects: {" << std::endl;

    for (const auto& object : _objects)
        object->log(stream) << "," << std::endl;

    return stream << "}";
}
