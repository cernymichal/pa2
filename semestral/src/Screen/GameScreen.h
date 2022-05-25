#pragma once

#include <memory>

#include "../Application.h"
#include "../Game.h"
#include "Screen.h"

/**
 * @brief gets input and call fixed update for game
 */
class GameScreen : public Screen {
public:
    explicit GameScreen(Application& application);

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
    char inputBuffer[3] = "  ";
    uint8_t inputIndex = 0;

    /**
     * @brief resets Screen variables and _dtAccumulator
     */
    void _resetScreen();

    /**
     * @brief resets input command buffer
     */
    void _resetInputBuffer();

    /**
     * @brief draws current command to the bottom left
     */
    void _drawInputBuffer();

    /**
     * @brief tries to execute command
     */
    void _commitInput();

    /**
     * @brief check if winTimer in Game hit 0
     */
    void _checkWin();
};
