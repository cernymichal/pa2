#pragma once

#include <list>
#include <map>
#include <memory>
#include <string>

#include "GameController.h"
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
    // TODO private
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

    bool ended() const;

    /**
     * @returns winner
     */
    const Player* getWinner() const;

    const std::string& mapName() const;

    GameController createPlayerController(uint8_t playerID);

    /**
     * @brief print debug info to stream
     *
     * @param[in] stream output stream
     */
    std::ostream& log(std::ostream& stream) const;

private:
    std::list<std::unique_ptr<GameObject>> m_objects;
    std::string m_mapName;

    Game();

    /**
     * @brief handle collision between objects
     */
    void collision();

    void pruneObjects();

    friend class GameBuilder;
    friend class GameController;
};
