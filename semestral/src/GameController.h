#pragma once

#include <list>

class Game;
class Player;
class AntNest;

class GameController {
public:
    /**
     * @param[in] playerId player to check
     *
     * @returns list of nests captured by player
     */
    std::list<AntNest*> getNests();

    /**
     * @brief disable all lines going from nest
     *
     * @param[in] playerId user, must own nest
     * @param[in] nestId
     */
    void disableLinesFrom(char nestId);

    /**
     * @brief activate AntLine from A to B
     *
     * @param[in] playerId user, must own nestA
     * @param[in] nestAId
     * @param[in] nestBId
     */
    void activateLine(char nestAId, char nestBId);

    /**
     * @brief activate AntLine from A to B
     *
     * @param[in] player user, must own nestA
     * @param[in] nestA
     * @param[in] nestB
     */
    void activateLine(AntNest* nestA, AntNest* nestB);

protected:
    GameController(Game* game, const Player* player);

private:
    Game* const m_game;
    const Player* const m_player;

    friend class Game;
};
