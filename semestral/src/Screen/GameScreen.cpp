#include "GameScreen.h"

#include <ncurses.h>

#include "../log.h"

const auto UPDATE_PERIOD = std::chrono::milliseconds(500);

GameScreen::GameScreen(Application& application) : Screen(application, "game screen") {
    m_timeoutDelay = 0;  // non blocking input
    resetScreen();
}

void GameScreen::update(std::chrono::nanoseconds dt, int key) {
    // PN_LOG("GameScreen::update(" << static_cast<long long>(dt.count()) << ", " << static_cast<char>(key) << ")");¨
    bool refreshNeeded = false;

    if (key == ';') {
        m_exit = true;
        m_paused = true;
        return;
    }
    else if (key == '\n') {
        commitInput();
        resetInputBuffer();
        drawInputBuffer();
        refreshNeeded = true;
    }
    else if (key == KEY_BACKSPACE && m_inputIndex > 0) {
        m_inputBuffer[--m_inputIndex] = ' ';
        drawInputBuffer();
        refreshNeeded = true;
    }
    else if (std::isalpha(key)) {
        if (m_inputIndex == 2)
            resetInputBuffer();

        m_inputBuffer[m_inputIndex++] = toupper(key);
        drawInputBuffer();
        refreshNeeded = true;
    }

    m_dtAccumulator += dt;

    if (m_dtAccumulator > UPDATE_PERIOD) {
        m_dtAccumulator -= UPDATE_PERIOD;
        m_application.m_state.game->update();

        clear();
        m_application.m_state.game->draw();

        drawInputBuffer();
        refreshNeeded = true;

        checkWin();
    }

    if (refreshNeeded)
        refresh();
}

void GameScreen::onExit() {
    if (m_paused) {
        resetScreen();
        m_application.openPauseScreen();
    }
    else
        m_application.openResultsScreen();
}

void GameScreen::resetScreen() {
    m_exit = false;
    m_paused = false;
    m_dtAccumulator = UPDATE_PERIOD;
}

void GameScreen::resetInputBuffer() {
    m_inputBuffer[0] = ' ';
    m_inputBuffer[1] = ' ';
    m_inputIndex = 0;
}

void GameScreen::drawInputBuffer() {
    mvaddstr(LINES - 1, COLS - 2, m_inputBuffer);
}

void GameScreen::commitInput() {
    PN_LOG("executing command: " << m_inputBuffer);

    if (m_inputIndex == 1 || m_inputBuffer[0] == m_inputBuffer[1])
        m_application.m_state.game->disableLinesFrom(0, m_inputBuffer[0]);
    else
        m_application.m_state.game->activateLine(0, m_inputBuffer[0], m_inputBuffer[1]);
}


void GameScreen::checkWin() {
    if(m_application.m_state.game->m_winTimer == 0) {
        PN_LOG("winner found");
        m_exit = true;
        m_paused = false;
    }
}
