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

    /**
     * @brief resets Screen variables and m_dtAccumulator
     */
    virtual void reset() override;

protected:
    virtual void onExit() override;

private:
    std::chrono::nanoseconds m_dtAccumulator;
    bool m_paused = false;
    char m_inputBuffer[3] = "  ";
    uint8_t m_inputIndex = 0;

    /**
     * @brief resets input command buffer
     */
    void resetInputBuffer();

    /**
     * @brief draws current command to the bottom left
     */
    void drawInputBuffer();

    /**
     * @brief tries to execute command
     */
    void commitInput();

    /**
     * @brief check if m_winTimer in Game hit 0
     */
    void checkWin();
};
