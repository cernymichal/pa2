#include "GameScreen.h"

#include <ncurses.h>

#include "../log.h"

using namespace std::literals::chrono_literals;

const auto g_updatePeriod = 500ms;

GameScreen::GameScreen(Application& application) : Screen(application, "game screen") {
    m_timeoutDelay = 0;  // non blocking input
    reset();
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

    if (m_dtAccumulator > g_updatePeriod) {
        m_dtAccumulator -= g_updatePeriod;
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

void GameScreen::reset() {
    Screen::reset();
    m_paused = false;
    m_dtAccumulator = g_updatePeriod;
}

void GameScreen::onExit() {
    if (m_paused) {
        reset();
        m_application.openPauseScreen();
    }
    else
        m_application.openResultsScreen();
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
    if (m_application.m_state.game->won()) {
        PN_LOG("winner found");
        m_exit = true;
        m_paused = false;
    }
}
