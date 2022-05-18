#include "GameScreen.h"

#include "../log.h"

const auto FRAME_TIME = std::chrono::milliseconds(500);

GameScreen::GameScreen(Application& application) : Screen(application, "game screen") {
    _timeoutDelay = 0;  // non blocking input
    _resetScreen();
}

void GameScreen::update(std::chrono::nanoseconds dt, int key) {
    // PN_LOG("GameScreen::update(" << (long long)dt.count() << ", " << (char)key << ")");

    switch (key) {
        case ERR:
            break;

        case KEY_BACKSPACE:
            exit = true;
            _paused = true;
            return;

        default:
            exit = true;
            return;
    }

    _dtAccumulator += dt;

    if (_dtAccumulator > FRAME_TIME) {
        _dtAccumulator -= FRAME_TIME;
        _application.state.game->update();
    }
}

void GameScreen::_onExit() {
    if (_paused) {
        _resetScreen();
        _application.openPauseScreen();
    }
    else
        _application.openResultsScreen();
}

void GameScreen::_resetScreen() {
    exit = false;
    _paused = false;
    _dtAccumulator = FRAME_TIME;
}
