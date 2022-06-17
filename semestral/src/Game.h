#pragma once

#include <filesystem>
#include <list>
#include <map>
#include <memory>
#include <string>

#include "GameObject/AntNest.h"
#include "GameObject/GameObject.h"

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
    int8_t m_winTimer = -1;

    Game();

    /**
     * @brief load Game from file
     *
     * - throws SaveException on fail
     *
     * @param[in] path save file
     */
    explicit Game(const std::filesystem::path& path);

    /**
     * @brief add GameObject to game
     */
    template <typename T, typename... Ts>
    void addObject(Ts... args) {
        addObject(new T(args...));
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
     * @brief create players in game
     *
     * - 1 * Player + aiPlayers * ComputerPlayers
     * - randomly assing starting points
     *
     * @param[in] aiPlayers count of ComputerPlayers
     */
    void createPlayers(uint8_t aiPlayers = 0);

    /**
     * @brief update all GameObjects and redraw
     */
    void update();

    /**
     * @brief count starting nests
     *
     * @return max number of players
     */
    uint8_t maxPlayers() const;

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

    /**
     * @brief draw all GameObjects
     */
    void draw();

    /**
     * @brief save Game to file
     *
     * - saves save name, map name and serializes all GameObjects
     * - throws SaveException on fail
     *
     * @param[in] path target
     */
    void save(const std::filesystem::path& path) const;

    /**
     * @brief print debug info to stream
     *
     * @param[in] stream output stream
     */
    std::ostream& log(std::ostream& stream) const;

    /**
     * @brief initialize map of GameObject names for loading
     */
    static void initGameObjectInstatiators();

private:
    std::list<std::unique_ptr<GameObject>> m_objects;

    /**
     * @brief handle collision between objects
     */
    void collision();
};
