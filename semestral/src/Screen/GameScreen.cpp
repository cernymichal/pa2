#include "GameScreen.h"

#include <ncurses.h>

#include "../utils/log.h"

using namespace std::literals::chrono_literals;

const auto g_updatePeriod = 500ms;

GameScreen::GameScreen(Application& application) : Screen(application, "game screen"),
                                                   m_playerInput(*m_application.m_state.game) {
    m_timeoutDelay = 0;  // non blocking input
    reset();
}

void GameScreen::update(std::chrono::nanoseconds dt, int key) {
    // PN_LOG("GameScreen::update(" << static_cast<long long>(dt.count()) << ", " << static_cast<char>(key) << ")");
    bool refreshNeeded = false;

    if (key == ';') {
        m_exit = true;
        m_paused = true;
        return;
    }
    else if (m_playerInput.input(key)) {
        refreshNeeded = true;
    }

    m_dtAccumulator += dt;
    if (m_dtAccumulator > g_updatePeriod) {
        m_dtAccumulator -= g_updatePeriod;
        fixedUpdate();
        refreshNeeded = true;
    }

    if (refreshNeeded) {
        mvaddstr(LINES - 1, COLS - 2, m_playerInput.bufferContent());
        refresh();
    }
}

void GameScreen::fixedUpdate() {
    m_application.m_state.game->update();

    clear();
    m_application.m_state.game->draw();

    if (m_application.m_state.game->ended()) {
        PN_LOG("winner found");
        m_exit = true;
        m_paused = false;
    }
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
