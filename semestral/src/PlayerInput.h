#pragma once

#include <stdint.h>

#include "GameController.h"

class PlayerInput {
public:
    PlayerInput(Game& game);

    const char* bufferContent() const;

    /**
     * @returns valid input
     */
    bool input(int key);

private:
    GameController* const m_gameController;
    char m_inputBuffer[3] = "  ";
    uint8_t m_inputIndex = 0;

    /**
     * @brief resets input command buffer
     */
    void resetInputBuffer();

    /**
     * @brief tries to execute command
     */
    void commitInput();
};
