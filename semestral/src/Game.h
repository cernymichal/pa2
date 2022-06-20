#pragma once

#include <list>
#include <map>
#include <memory>
#include <string>

#include "GameObject/AntNest.h"
#include "GameObject/GameObject.h"

class GameBuilder;

/**
 * @brief holds all game objects and logic
 *
 * - m_mapName - original loaded map name, for saving
 * - m_nestMap - map of nest ids to AntNest*
 * - m_playerMap - map of player ids to Player*
 * - m_winTimer - if 0 after update game ends
 */
class Game {
public:
    std::string m_mapName;
    std::map<char, AntNest*> m_nestMap;
    std::map<uint8_t, Player*> m_playerMap;

    /**
     * @brief add GameObject to game
     */
    template <typename T, typename... Ts>
    void addObject(Ts&&... args) {
        addObject(new T(std::forward<Ts>(args)...));
    }

    /**
     * @brief add already constructed GameObject to game
     *
     * @param[in] object pointer to GameObject, will be managed by Game
     */
    void addObject(GameObject* object);

    /**
     * @brief called from outside after game is constructed
     */
    void onLoad();

    /**
     * @brief update all GameObjects and redraw
     */
    void update();

    /**
     * @brief draw all GameObjects
     */
    void draw();

    /**
     * @brief disable all lines going from nest
     *
     * @param[in] playerId user, must own nest
     * @param[in] nestId
     */
    void disableLinesFrom(uint8_t playerId, char nestId);

    /**
     * @brief activate AntLine from A to B
     *
     * @param[in] playerId user, must own nestA
     * @param[in] nestAId
     * @param[in] nestBId
     */
    void activateLine(uint8_t playerId, char nestAId, char nestBId);

    /**
     * @brief activate AntLine from A to B
     *
     * @param[in] player user, must own nestA
     * @param[in] nestA
     * @param[in] nestB
     */
    void activateLine(Player* player, AntNest* nestA, AntNest* nestB);

    /**
     * @param[in] playerId player to check
     *
     * @returns list of nests captured by player
     */
    std::list<AntNest*> getNests(uint8_t playerId);

    /**
     * @brief check if winner can be decided
     *
     * - will start a timer after which the winner will or won't be decided
     */
    void checkWin();

    /**
     * if only one player has nests, return them
     *
     * @returns winner
     */
    Player* getWinner();

    bool won() const;

    const std::string& mapName() const;

    /**
     * @brief print debug info to stream
     *
     * @param[in] stream output stream
     */
    std::ostream& log(std::ostream& stream) const;

private:
    int8_t m_winTimer = -1;
    std::list<std::unique_ptr<GameObject>> m_objects;

    Game();

    /**
     * @brief handle collision between objects
     */
    void collision();

    friend class GameBuilder;
};
