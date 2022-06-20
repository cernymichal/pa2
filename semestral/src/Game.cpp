#include "Game.h"

#include <iostream>
#include <vector>

#include "GameObject/AntLine.h"
#include "GameObject/ComputerPlayer.h"
#include "log.h"
#include "utils.cpp"

Game::Game() {
}

void Game::addObject(GameObject* object) {
    auto iter = m_objects.begin();
    for (; iter != m_objects.end() && (*iter)->updatePriority() < object->updatePriority(); iter++)
        ;

    m_objects.emplace(iter, object);

    object->onAdd(this);
}

void Game::onLoad() {
    checkWin();

    for (auto& object : m_objects)
        object->onLoad();
}

void Game::update() {
    for (auto& object : m_objects)
        object->update();

    collision();

    for (auto iterA = m_objects.begin(); iterA != m_objects.end(); iterA++) {
        if ((*iterA)->dead())
            iterA = m_objects.erase(iterA);
    }

    if (m_winTimer > 0)
        m_winTimer--;
    else if (m_winTimer == 0 && !getWinner())
        m_winTimer = -1;
}

void Game::collision() {
    for (auto iterA = m_objects.begin(); iterA != m_objects.end(); iterA++) {
        auto iterB = iterA;
        for (iterB++; iterB != m_objects.end(); iterB++) {
            if ((*iterA)->m_hitDistance < 0 || (*iterB)->m_hitDistance < 0)
                continue;

            auto d = distance((*iterA)->location(), (*iterB)->location());
            if (d <= std::max((*iterA)->m_hitDistance, (*iterB)->m_hitDistance)) {
                (*iterA)->collideWith(**iterB);
                (*iterB)->collideWith(**iterA);
            }
        }
    }
}

void Game::draw() {
    for (auto& object : m_objects)
        object->draw();
}

std::list<AntNest*> Game::getNests(uint8_t playerId) {
    std::list<AntNest*> nests;

    for (auto& nest : m_nestMap) {
        if (nest.second->player() && nest.second->player()->playerId() == playerId)
            nests.push_back(nest.second);
    }

    return nests;
}

void Game::disableLinesFrom(uint8_t playerId, char nestId) {
    auto nest = m_nestMap.find(nestId);

    if (nest == m_nestMap.end() || !nest->second->player() || nest->second->player()->playerId() != playerId)
        return;

    nest->second->disableLines();
}

void Game::activateLine(uint8_t playerId, char nestAId, char nestBId) {
    auto nest = m_nestMap.find(nestAId);

    if (nest == m_nestMap.end() || !nest->second->player() || nest->second->player()->playerId() != playerId)
        return;

    auto line = nest->second->m_lineMap.find(nestBId);

    if (line == nest->second->m_lineMap.end())
        return;

    nest->second->disableLines();
    line->second->switchSide(nest->second, true);
}

void Game::activateLine(Player* player, AntNest* nestA, AntNest* nestB) {
    if (nestA->player() != player)
        return;

    auto line = nestA->m_lineMap.find(nestB->nestId());

    if (line == nestA->m_lineMap.end())
        return;

    nestA->disableLines();
    line->second->switchSide(nestA, true);
}

Player* Game::getWinner() {
    Player* winner = nullptr;

    for (const auto& nest : m_nestMap) {
        if (!winner)
            winner = nest.second->player();
        else if (nest.second->player() && nest.second->player() != winner)
            return nullptr;
    }

    return winner;
}

void Game::checkWin() {
    if (!getWinner())
        return;

    m_winTimer = 10;  // 5s with period of .5s
}

bool Game::won() const {
    return m_winTimer == 0;
}

const std::string& Game::mapName() const {
    return m_mapName;
}

std::ostream& Game::log(std::ostream& stream) const {
    stream << "objects: {" << std::endl;

    for (const auto& object : m_objects)
        object->log(stream) << "," << std::endl;

    return stream << "}";
}
