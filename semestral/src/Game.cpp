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
    for (auto& object : m_objects)
        object->onLoad();
}

void Game::update() {
    for (auto& object : m_objects)
        object->update();

    collision();

    pruneObjects();
}

void Game::collision() {
    for (auto iterA = m_objects.begin(); iterA != m_objects.end(); iterA++) {
        auto iterB = iterA;
        for (iterB++; iterB != m_objects.end(); iterB++) {
            if (!(*iterA)->collisionEnabled() || !(*iterB)->collisionEnabled())
                continue;

            if (((*iterA)->location() - (*iterB)->location()) == Vector2<uint8_t>(0, 0)) {
                (*iterA)->collideWith(**iterB);
                (*iterB)->collideWith(**iterA);
            }
        }
    }
}

void Game::pruneObjects() {
    for (auto iterA = m_objects.begin(); iterA != m_objects.end();) {
        if ((*iterA)->destroyed()) {
            (*iterA)->onErase();
            iterA = m_objects.erase(iterA);
        }
        else
            iterA++;
    }
}

void Game::draw() {
    for (auto& object : m_objects)
        object->draw();
}

bool Game::ended() const {
    uint8_t notDefeated = 0;

    for (const auto& el : m_playerMap) {
        if (!el.second->defeated())
            notDefeated++;
    }

    return notDefeated < 2;
}

const Player* Game::getWinner() const {
    Player* winner = nullptr;

    for (const auto& el : m_playerMap) {
        if (!el.second->defeated()) {
            if (!winner)
                winner = el.second;
            else
                return nullptr;
        }
    }

    return winner;
}

const std::string& Game::mapName() const {
    return m_mapName;
}

GameController Game::createPlayerController(uint8_t playerID) {
    return GameController(this, m_playerMap.at(playerID));
}

std::ostream& Game::log(std::ostream& stream) const {
    stream << "objects: {" << std::endl;

    for (const auto& object : m_objects)
        object->log(stream) << "," << std::endl;

    return stream << "}";
}
