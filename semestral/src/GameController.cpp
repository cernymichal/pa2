#include "GameController.h"

#include "Game.h"
#include "GameObject/AntLine.h"

GameController::GameController(Game* game, const Player* player) : m_game(game), m_player(player) {
}

std::list<AntNest*> GameController::getNests() {
    std::list<AntNest*> nests;

    for (auto& nest : m_game->m_nestMap) {
        if (nest.second->player() && nest.second->player()->playerId() == m_player->playerId())
            nests.push_back(nest.second);
    }

    return nests;
}

void GameController::disableLinesFrom(char nestId) {
    auto nest = m_game->m_nestMap.find(nestId);

    if (nest == m_game->m_nestMap.end() || !nest->second->player() || nest->second->player()->playerId() != m_player->playerId())
        return;

    nest->second->disableLines();
}

void GameController::activateLine(char nestAId, char nestBId) {
    auto nest = m_game->m_nestMap.find(nestAId);

    if (nest == m_game->m_nestMap.end() || !nest->second->player() || nest->second->player()->playerId() != m_player->playerId())
        return;

    auto line = nest->second->m_lineMap.find(nestBId);

    if (line == nest->second->m_lineMap.end())
        return;

    nest->second->disableLines();
    line->second->switchSide(nest->second, true);
}

void GameController::activateLine(AntNest* nestA, AntNest* nestB) {
    if (nestA->player() != m_player)
        return;

    auto line = nestA->m_lineMap.find(nestB->nestId());

    if (line == nestA->m_lineMap.end())
        return;

    nestA->disableLines();
    line->second->switchSide(nestA, true);
}
