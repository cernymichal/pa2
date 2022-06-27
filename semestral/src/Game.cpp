#include "Game.h"

#include <iostream>

#include "utils/log.h"

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

    for (auto iterA = m_objects.begin(); iterA != m_objects.end();) {
        if ((*iterA)->dead())
            iterA = m_objects.erase(iterA);
        else
            iterA++;
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

            auto d = ((*iterA)->location() - (*iterB)->location()).length();
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
