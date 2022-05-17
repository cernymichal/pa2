#pragma once

#include <memory>

#include "../Application.h"
#include "../Game.h"
#include "Screen.h"

class GameScreen : public Screen {
public:
    GameScreen(Application& application, const std::shared_ptr<Game>& game);

    /**
     * @brief update game
     *
     * @param[in] dt delta time since last update
     * @param[in] key ncurses input key
     */
    virtual void update(std::chrono::nanoseconds dt, int key) override;

protected:
    virtual void _onExit() override;

private:
    std::shared_ptr<Game> _game;
    std::chrono::nanoseconds _dtAccumulator;
    bool _paused = false;

    void _resetScreen();
};
