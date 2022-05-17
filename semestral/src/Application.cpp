#include "Application.h"

#include "Save.h"
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

    auto onExit = [](Dialog<std::string>& dialog, Application& application) {
        if (dialog.closed)
            return;

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
        new Dialog<std::string>(
            *this, "pins & needles", onExit,
            {"new game",
             "load",
             "exit"},
            true));
}

void Application::openMapListScreen() {
    PN_LOGH2("opening mapListScreen");

    auto onExit = [](Dialog<Save>& dialog, Application& application) {
        if (dialog.closed) {
            application.openMainMenuScreen();
            return;
        }

        dialog.exit = false;

        // TODO load map
        application.state.game = std::make_shared<Game>();
        application.openOponentNumberScreen();
    };

    openScreen(new Dialog<Save>(*this, "map list screen", onExit, Save::findMaps()));
}

void Application::openSaveListScreen() {
    PN_LOGH2("opening saveListScreen");

    auto onExit = [](Dialog<Save>& dialog, Application& application) {
        if (dialog.closed) {
            application.openMainMenuScreen();
            return;
        }

        application.state.game = std::make_shared<Game>(
            Game::load(dialog.options[dialog.optionIndex].path));
        application.openGameScreen();
    };

    openScreen(new Dialog<Save>(*this, "save list screen", onExit, Save::findSaves()));
}

void Application::openOponentNumberScreen() {
    PN_LOGH2("opening oponentNumberScreen");

    // TODO get max oponenents from map
    std::vector<std::string> oponents({"1", "2", "3"});

    auto onExit = [](Dialog<std::string>& dialog, Application& application) {
        if (dialog.closed)
            return;

        application.closeCurrentScreen();  // close self
        application.closeCurrentScreen();  // close map screen

        // TODO create computer players
        application.openGameScreen();
    };

    openScreen(new Dialog<std::string>(*this, "oponent number screen", onExit, oponents));
}

void Application::openGameScreen() {
    PN_LOGH2("opening gameScreen");

    state.game->addObject(new Ant(0, LINES / 2));

    PN_LOG_OBJ(state.game);

    openScreen(new GameScreen(*this, state.game));
}

void Application::openPauseScreen() {
    PN_LOGH2("opening pauseScreen");

    auto onExit = [](Dialog<std::string>& dialog, Application& application) {
        if (dialog.closed)
            return;

        switch (dialog.optionIndex) {
            case 0:
                break;
            case 1:
                // TODO save name
                application.state.game->save(Save::createSavePath("TODO"));
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
        new Dialog<std::string>(
            *this, "pause screen", onExit,
            {"continue",
             "save",
             "surrender",
             "return to main menu"}));
}

void Application::openResultsScreen() {
    PN_LOGH2("opening resultsScreen");

    // TODO results screen

    auto onExit = [](Dialog<std::string>& dialog, Application& application) {
        application.openMainMenuScreen();
    };

    openScreen(new Dialog<std::string>(*this, "results screen", onExit, {"return to main menu"}));
}

void Application::_run() {
    while (!_screens.empty()) {
        if (_screens.top()->exit) {
            closeCurrentScreen();
            continue;
        }

        PN_LOG("showing " << _screens.top()->title);

        _screens.top()->show();
    }
}
