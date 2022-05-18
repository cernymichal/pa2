#pragma once

#include <memory>

#include "../Application.h"
#include "../Game.h"
#include "Screen.h"

/**
 * @brief get input and call fixed update for game
 */
class GameScreen : public Screen {
public:
    GameScreen(Application& application);

    /**
     * @brief handle input, periodically call game update
     *
     * @param[in] dt delta time since last update
     * @param[in] key ncurses input key
     */
    virtual void update(std::chrono::nanoseconds dt, int key) override;

protected:
    virtual void _onExit() override;

private:
    std::chrono::nanoseconds _dtAccumulator;
    bool _paused = false;

    void _resetScreen();
};