#include "GameScreen.h"

#include <ncurses.h>

#include "../log.h"

const auto FRAME_TIME = std::chrono::milliseconds(500);

GameScreen::GameScreen(Application& application) : Screen(application, "game screen") {
    _timeoutDelay = 0;  // non blocking input
    _resetScreen();
}

void GameScreen::update(std::chrono::nanoseconds dt, int key) {
    // PN_LOG("GameScreen::update(" << (long long)dt.count() << ", " << (char)key << ")");¨
    bool refreshNeeded = false;

    if (key == ';') {
        exit = true;
        _paused = true;
        return;
    }
    else if (key == '\n') {
        _commitInput();
        _resetInputBuffer();
        _drawInputBuffer();
        refreshNeeded = true;
    }
    else if (key == KEY_BACKSPACE && inputIndex > 0) {
        inputBuffer[--inputIndex] = ' ';
        _drawInputBuffer();
        refreshNeeded = true;
    }
    else if (std::isalpha(key)) {
        if (inputIndex == 2)
            _resetInputBuffer();

        inputBuffer[inputIndex++] = toupper(key);
        _drawInputBuffer();
        refreshNeeded = true;
    }

    _dtAccumulator += dt;

    if (_dtAccumulator > FRAME_TIME) {
        _dtAccumulator -= FRAME_TIME;
        _application.state.game->update();

        clear();
        _application.state.game->draw();

        _drawInputBuffer();
        refreshNeeded = true;
    }

    if (refreshNeeded)
        refresh();
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

void GameScreen::_resetInputBuffer() {
    inputBuffer[0] = ' ';
    inputBuffer[1] = ' ';
    inputIndex = 0;
}

void GameScreen::_drawInputBuffer() {
    mvaddstr(LINES - 1, COLS - 2, inputBuffer);
}

void GameScreen::_commitInput() {
    if (inputIndex == 1 || inputBuffer[0] == inputBuffer[1])
        _application.state.game->disableLinesFrom(0, inputBuffer[0]);
    else
        _application.state.game->activateLine(0, inputBuffer[0], inputBuffer[1]);
}
