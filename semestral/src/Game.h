#pragma once

#include <memory>
#include <vector>

#include "GameObject.h"

class Game {
    std::vector<std::unique_ptr<GameObject>> _objects;

public:
    /**
     * @brief register GameObject to game
     *
     * @param[in] object pointer to GameObject, will be managed by Game
     */
    void addObject(GameObject *object);

    /**
     * @brief update all GameObjects
     */
    void update();

    /**
     * @brief draw all GameObjects
     */
    void draw();

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
};
