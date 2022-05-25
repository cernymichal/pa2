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
 * - mapName - original loaded map name, for saving
 * - nestMap - map of nest ids to AntNest*
 * - playerMap - map of player ids to Player*
 * - winTimer - if 0 after update game ends
 */
class Game {
public:
    std::string mapName;
    std::map<char, AntNest*> nestMap;
    std::map<uint8_t, Player*> playerMap;
    int8_t winTimer = -1;

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
     * @brief register GameObject to game
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
     * - 1 x Player + aiPlayers x ComputerPlayers
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
     * @brief initialize map of GameObject headers for loading
     */
    static void initGONameMap();

private:
    std::list<std::unique_ptr<GameObject>> _objects;

    /**
     * @brief handle collision between objects
     */
    void _collision();
};
