#pragma once

#include <list>
#include <map>
#include <memory>
#include <string>

#include "GameObject/AntNest.h"
#include "GameObject/GameObject.h"

class GameBuilder;
class GameController;

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
    friend class GameController;
};
