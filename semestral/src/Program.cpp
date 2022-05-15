#include "Program.h"

#include "game_object/Ant.h"
#include "game_object/Label.h"
#include "log.h"
#include "screen/Dialog.h"
#include "screen/Game.h"

Program::Program() {
}

void Program::start() {
    PN_LOGH1("Program::start()");

    Screen::initNCurses();

    _mainMenuScreen();

    Screen::exitNCurses();
}

void Program::_mainMenuScreen() {
    PN_LOGH2("entered Program::_mainMenuScreen");

    while (true) {
        _toMainMenu = false;

        Dialog dialog({"new game", "load", "exit"}, "pins & needles");
        dialog.show();

        if (dialog.closed || dialog.optionIndex == 2)
            break;

        switch (dialog.optionIndex) {
            case 0:
                _mapListScreen();
                break;
            case 1:
                _saveListScreen();
                break;
        }
    }
}

void Program::_mapListScreen() {
    PN_LOGH2("entered Program::_mapListScreen");

    while (!_toMainMenu) {
        // TODO search for maps
        std::vector<std::string> maps({"map 1", "map 2", "map 3"});

        Dialog dialog(maps);
        dialog.show();

        if (dialog.closed)
            break;

        // TODO load map
        _game.clear();
        _oponentNumberScreen();
    }
}

void Program::_saveListScreen() {
    PN_LOGH2("entered Program::_saveListScreen");

    // TODO search for saves
    std::vector<std::string> saves({"save 1", "save 2", "save 3"});

    Dialog dialog(saves);
    dialog.show();

    if (dialog.closed)
        return;

    // TODO load save
    _gameScreen();
}

void Program::_oponentNumberScreen() {
    PN_LOGH2("entered Program::_oponentNumberScreen");

    // TODO get max oponenents from map
    std::vector<std::string> oponents({"1", "2", "3"});

    Dialog dialog(oponents);
    dialog.show();

    if (dialog.closed)
        return;

    // TODO create computer players
    _gameScreen();
}

void Program::_gameScreen() {
    PN_LOGH2("entered Program::_gameScreen");

    _game.addObject(new Ant(0, LINES / 2));

    PN_LOG_OBJ(&_game);

    while (true) {
        _game.resetScreen();
        _game.show();

        if (!_game.paused)
            break;

        // TODO pause screen communication
        _pauseScreen();
    }

    // TODO results screen if
    _resultsScreen();
}

void Program::_pauseScreen() {
    PN_LOGH2("entered Program::_pauseScreen");

    Dialog dialog({"continue", "save", "surrender", "return to main menu"});
    dialog.show();

    if (dialog.closed)
        return;

    switch (dialog.optionIndex) {
        case 0:
            break;
        case 1:
            // TODO save
            break;
        case 2:
            // TODO surrender
            break;
        case 3:
            // TODO return to menu
            break;
    }
}

void Program::_resultsScreen() {
    PN_LOGH2("entered Program::_resultsScreen");

    // TODO results screen

    Dialog dialog({"return to main menu"});
    dialog.show();

    _game.clear();
    _toMainMenu = true;
}
