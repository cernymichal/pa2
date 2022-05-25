#include "Application.h"

#include "GameObject/ComputerPlayer.h"
#include "Save.h"
#include "Screen/Dialog.h"
#include "Screen/GameScreen.h"
#include "Screen/SaveList.h"
#include "log.h"

Application::Application() {
}

void Application::start() {
    PN_LOGH1("Application::start()");

    Game::initGONameMap();
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
                if (Save::emptyMapDirectory()) {
                    PN_LOG("no maps found");

                    dialog.exit = false;

                    break;
                }
                application.openMapListScreen();
                break;

            case 1:
                if (Save::emptySaveDirectory()) {
                    PN_LOG("no saves found");

                    dialog.exit = false;

                    break;
                }
                application.openSaveListScreen();
                break;
        }
    };

    openScreen(
        new Dialog<std::string>(
            *this, "pins & needles",
            {"new game",
             "load",
             "exit"},
            onExit,
            true));
}

void Application::openMapListScreen() {
    PN_LOGH2("opening mapListScreen");

    auto onExit = [](Dialog<Save>& dialog, Application& application) {
        if (dialog.closed) {
            application.openMainMenuScreen();
            return;
        }

        try {
            application.state.game = std::make_unique<Game>(dialog.options[dialog.optionIndex].path);
        } catch (SaveException& _) {
            application.openMainMenuScreen();
            return;
        }

        dialog.exit = false;

        application.openOponentNumberScreen();
    };

    openScreen(new Dialog<Save>(*this, "map list screen", Save::findMaps(), onExit));
}

void Application::openSaveListScreen() {
    PN_LOGH2("opening saveListScreen");

    auto onExit = [](Dialog<Save>& dialog, Application& application) {
        if (dialog.closed) {
            application.openMainMenuScreen();
            return;
        }

        try {
            application.state.game = std::make_unique<Game>(dialog.options[dialog.optionIndex].path);
        } catch (SaveException& _) {
            application.openMainMenuScreen();
            return;
        }

        application.openGameScreen();
    };

    openScreen(new SaveList(*this, Save::findSaves(), onExit));
}

void Application::openOponentNumberScreen() {
    PN_LOGH2("opening oponentNumberScreen");

    auto oponents = std::vector<std::string>();

    auto max = state.game->maxPlayers();
    for (uint8_t i = 0; i < max; i++)
        oponents.push_back(std::to_string(i));

    auto onExit = [](Dialog<std::string>& dialog, Application& application) {
        if (dialog.closed)
            return;

        application.closeCurrentScreen();  // close self
        application.closeCurrentScreen();  // close map screen

        application.state.game->createPlayers(dialog.optionIndex);

        application.openGameScreen();
    };

    openScreen(new Dialog<std::string>(*this, "oponent number screen", oponents, onExit));
}

void Application::openGameScreen() {
    PN_LOGH2("opening gameScreen");

    PN_LOG_OBJ(state.game);

    state.game->onLoad();

    openScreen(new GameScreen(*this));
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
                try {
                    application.state.game->save(
                        Save::createSavePath(application.state.game->mapName));
                } catch (SaveException& _) {
                    dialog.exit = false;
                }
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
            *this, "pause screen",
            {"continue",
             "save",
             "surrender",
             "return to main menu"},
            onExit));
}

void Application::openResultsScreen() {
    PN_LOGH2("opening resultsScreen");

    std::string text = "surrendered";

    auto winner = state.game->getWinner();
    if (winner)
        text = std::string(winner->name).append(" won");

    auto onExit = [](Dialog<std::string>& dialog, Application& application) {
        application.openMainMenuScreen();
    };

    openScreen(new Dialog<std::string>(*this, text, {"return to main menu"}, onExit, true));
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
