#pragma once

#include <list>
#include <memory>

#include "game_object/GameObject.h"

class Game {
public:
    Game();

    /**
     * @brief register GameObject to game
     *
     * @param[in] object pointer to GameObject, will be managed by Game
     */
    void addObject(GameObject *object);

    /**
     * @brief update all GameObjects and redraw
     */
    void update();

    /**
     * @brief save Game to file
     *
     * @param[in] fileName target
     *
     * @return success
     */
    bool save(const char *fileName) const;

    /**
     * @brief load Game from file
     *
     * @param[in] fileName source
     *
     * @return loaded Game
     */
    static Game load(const char *fileName);

    /**
     * @brief print debug info to stream
     *
     * @param[in] stream output stream
     */
    std::ostream &log(std::ostream &stream) const;

private:
    std::list<std::unique_ptr<GameObject>> _objects;

    /**
     * @brief draw all GameObjects
     */
    void _draw();
};
