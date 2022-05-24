#include "Game.h"

#include <ncurses.h>

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
    std::vector<Player*> players;
    for (const auto& player : playerMap)
        players.push_back(player.second);

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(players.begin(), players.end(), g);

    auto playerIter = players.begin();
    auto nestIter = nestMap.begin();
    for (; playerIter != players.end(); playerIter++, nestIter++)
        nestIter->second->changePlayer(*playerIter);
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
            auto d = distance((*iterA)->x, (*iterB)->x, (*iterA)->y, (*iterB)->y);
            if (d < std::max((*iterA)->hitDistance, (*iterB)->hitDistance)) {
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

    for (const auto& nest : nestMap) {
        if (nest.second->starting)
            startingPoints++;
    }

    return startingPoints;
}

std::ostream& Game::log(std::ostream& stream) const {
    stream << "objects: {" << std::endl;

    for (const auto& object : _objects)
        object->log(stream) << "," << std::endl;

    return stream << "}";
}
