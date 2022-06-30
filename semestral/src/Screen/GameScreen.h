#pragma once

#include "../Application.h"
#include "../Game.h"
#include "../PlayerInput.h"
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

    void fixedUpdate();

    /**
     * @brief resets Screen variables and m_dtAccumulator
     */
    virtual void reset() override;

protected:
    virtual void onExit() override;

private:
    std::chrono::nanoseconds m_dtAccumulator;
    bool m_paused = false;
    PlayerInput m_playerInput;
};
