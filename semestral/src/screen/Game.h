#pragma once

#include <memory>
#include <list>

#include "../game_object/GameObject.h"
#include "Screen.h"

class Game : public Screen {
    std::list<std::unique_ptr<GameObject>> _objects;
    std::chrono::nanoseconds _dtAccumulator;

public:
    bool paused = false;

    Game();

    /**
     * @brief reset screen variables
     */
    void resetScreen();

    /**
     * @brief free objects and reset state
     */
    void clear();

    /**
     * @brief register GameObject to game
     *
     * @param[in] object pointer to GameObject, will be managed by Game
     */
    void addObject(GameObject *object);

    /**
     * @brief update all GameObjects and redraw
     *
     * @param[in] dt delta time since last update
     * @param[in] key ncurses input key
     */
    virtual void update(std::chrono::nanoseconds dt, int key) override;

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
    /**
     * @brief draw all GameObjects
     */
    void _draw();
};
