#include "Program.h"

#include "game_object/Ant.h"
#include "game_object/Label.h"
#include "log.h"
#include "screen/Dialog.h"
#include "screen/Game.h"

Program::Program() {
}

void Program::start() {
    Screen::initNCurses();

    _mainMenuScreen();

    Screen::exitNCurses();
}

void Program::_mainMenuScreen() {
    while (true) {
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
    // TODO close after game ends
    while (true) {
        // TODO search for maps
        std::vector<std::string> maps({"map 1", "map 2", "map 3"});

        Dialog dialog(maps);
        dialog.show();

        if (dialog.closed)
            break;

        // TODO load map
        _oponentNumberScreen();
    }
}

void Program::_saveListScreen() {
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
    // TODO game screen

    Dialog dialog({}, "*game here*");
    dialog.show();
}

void Program::_pauseScreen() {
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
    // TODO results screen

    Dialog dialog({"return to main menu"});
    dialog.show();
}
