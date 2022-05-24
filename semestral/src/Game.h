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
 */
class Game {
public:
    std::string mapName;
    std::map<char, AntNest*> nestMap;
    std::map<uint8_t, Player*> playerMap;

    /**
     * @brief load Game from file
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

    void onLoad();

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
     * @brief draw all GameObjects
     */
    void draw();

    /**
     * @brief save Game to file
     *
     * @param[in] path target
     *
     * @return success
     */
    bool save(const std::filesystem::path& path) const;

    void disableLinesFrom(uint8_t playerId, char nestId);

    void activateLine(uint8_t playerId, char nestAId, char nestBId);

    /**
     * @brief print debug info to stream
     *
     * @param[in] stream output stream
     */
    std::ostream& log(std::ostream& stream) const;

    static void initGONameMap();

private:
    std::list<std::unique_ptr<GameObject>> _objects;

    /**
     * @brief handle collision between objects
     */
    void _collision();
};
