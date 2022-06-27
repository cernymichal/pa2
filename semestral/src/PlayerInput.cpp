#include "PlayerInput.h"

#include <ncurses.h>

#include "Game.h"
#include "GameController.h"
#include "utils/log.h"

PlayerInput::PlayerInput(Game& game) : m_gameController(game.m_playerMap[0]->m_gameController.get()) {
}

const char* PlayerInput::bufferContent() const {
    return m_inputBuffer;
}

bool PlayerInput::input(int key) {
    if (key == '\n') {
        commitInput();
        resetInputBuffer();

        return true;
    }
    else if (key == KEY_BACKSPACE && m_inputIndex > 0) {
        m_inputBuffer[--m_inputIndex] = ' ';

        return true;
    }
    else if (std::isalpha(key)) {
        if (m_inputIndex == 2)
            resetInputBuffer();

        m_inputBuffer[m_inputIndex++] = toupper(key);

        return true;
    }

    return false;
}

void PlayerInput::resetInputBuffer() {
    m_inputBuffer[0] = ' ';
    m_inputBuffer[1] = ' ';
    m_inputIndex = 0;
}

void PlayerInput::commitInput() {
    PN_LOG("PlayerInput::commitInput() - \"" << m_inputBuffer << "\"");

    if (m_inputIndex == 1 || m_inputBuffer[0] == m_inputBuffer[1])
        m_gameController->disableLinesFrom(m_inputBuffer[0]);
    else
        m_gameController->activateLine(m_inputBuffer[0], m_inputBuffer[1]);
}
