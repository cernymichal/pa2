#include "Application.h"

#include "game_object/Ant.h"
#include "log.h"
#include "screen/Dialog.h"
#include "screen/GameScreen.h"

Application::Application() {
}

void Application::start() {
    PN_LOGH1("Application::start()");

    Screen::initNCurses();

    openMainMenuScreen();
    _run();

    Screen::exitNCurses();
}

void Application::openScreen(Screen* screen) {
    if (!_screens.empty() && _screens.top()->exit)
        closeCurrentScreen();

    _screens.emplace(screen);
}

void Application::closeCurrentScreen() {
    _screens.pop();
}

void Application::openMainMenuScreen() {
    PN_LOGH2("opening mainMenuScreen");

    state = ApplicationState();

    auto onExit = [](Dialog& dialog, Application& application) {
        switch (dialog.optionIndex) {
            case 0:
                application.openMapListScreen();
                break;
            case 1:
                application.openSaveListScreen();
                break;
        }
    };

    openScreen(
        new Dialog(
            *this, "pins & needles", onExit,
            {"new game",
             "load",
             "exit"},
            true));
}

void Application::openMapListScreen() {
    PN_LOGH2("opening mapListScreen");

    // TODO search for maps
    std::vector<std::string> maps({"map 1", "map 2", "map 3"});

    auto onExit = [](Dialog& dialog, Application& application) {
        if (dialog.closed) {
            application.openMainMenuScreen();
            return;
        }

        dialog.exit = false;

        // TODO load map
        application.state.game = std::make_shared<Game>();
        application.openOponentNumberScreen();
    };

    openScreen(new Dialog(*this, "map list screen", onExit, maps));
}

void Application::openSaveListScreen() {
    PN_LOGH2("opening saveListScreen");

    // TODO search for saves
    std::vector<std::string> saves({"save 1", "save 2", "save 3"});

    auto onExit = [](Dialog& dialog, Application& application) {
        if (dialog.closed) {
            application.openMainMenuScreen();
            return;
        }

        // TODO load save
        application.state.game = std::make_shared<Game>();
        application.openGameScreen();
    };

    openScreen(new Dialog(*this, "save list screen", onExit, saves));
}

void Application::openOponentNumberScreen() {
    PN_LOGH2("opening oponentNumberScreen");

    // TODO get max oponenents from map
    std::vector<std::string> oponents({"1", "2", "3"});

    auto onExit = [](Dialog& dialog, Application& application) {
        if (dialog.closed)
            return;

        application.closeCurrentScreen();  // close self
        application.closeCurrentScreen();  // close map screen

        // TODO create computer players
        application.openGameScreen();
    };

    openScreen(new Dialog(*this, "oponent number screen", onExit, oponents));
}

void Application::openGameScreen() {
    PN_LOGH2("opening gameScreen");

    state.game->addObject(new Ant(0, LINES / 2));

    PN_LOG_OBJ(state.game);

    openScreen(new GameScreen(*this, state.game));
}

void Application::openPauseScreen() {
    PN_LOGH2("opening pauseScreen");

    auto onExit = [](Dialog& dialog, Application& application) {
        if (dialog.closed)
            return;

        switch (dialog.optionIndex) {
            case 0:
                break;
            case 1:
                // TODO save
                dialog.exit = false;
                break;
            case 2:
                application.closeCurrentScreen();  // close self
                application.closeCurrentScreen();  // close game screen
                application.openResultsScreen();
                break;
            case 3:
                application.closeCurrentScreen();  // close self
                application.closeCurrentScreen();  // close game screen
                application.openMainMenuScreen();
                break;
        }
    };

    openScreen(
        new Dialog(
            *this, "pause screen", onExit,
            {"continue",
             "save",
             "surrender",
             "return to main menu"}));
}

void Application::openResultsScreen() {
    PN_LOGH2("opening resultsScreen");

    // TODO results screen

    auto onExit = [](Dialog& dialog, Application& application) {
        application.openMainMenuScreen();
    };

    openScreen(new Dialog(*this, "results screen", onExit, {"return to main menu"}));
}

void Application::_run() {
    while (!_screens.empty()) {
        if (_screens.top()->exit) {
            closeCurrentScreen();
            continue;
        }

        _screens.top()->show();
    }
}
