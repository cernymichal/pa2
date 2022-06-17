#include "Application.h"

#include "GameObject/ComputerPlayer.h"
#include "Save.h"
#include "Screen/Dialog.cpp"
#include "Screen/GameScreen.h"
#include "Screen/SaveList.h"
#include "log.h"

Application::Application() {
}

void Application::start() {
    PN_LOGH1("Application::start()");

    Game::initGameObjectInstatiators();
    Screen::initNCurses();

    openMainMenuScreen();
    run();

    Screen::exitNCurses();
}

void Application::openScreen(Screen* screen) {
    if (!m_screens.empty() && m_screens.top()->m_exit)
        closeCurrentScreen();

    m_screens.emplace(screen);
}

void Application::closeCurrentScreen() {
    m_screens.pop();
}

void Application::openMainMenuScreen() {
    PN_LOGH2("opening mainMenuScreen");

    m_state = ApplicationState();

    auto onExit = [](Dialog<std::string>& dialog, Application& application) {
        if (dialog.m_closed)
            return;

        switch (dialog.m_optionIndex) {
            case 0:
                if (Save::emptyMapDirectory()) {
                    PN_LOG("no maps found");

                    dialog.m_exit = false;

                    break;
                }
                application.openMapListScreen();
                break;

            case 1:
                if (Save::emptySaveDirectory()) {
                    PN_LOG("no saves found");

                    dialog.m_exit = false;

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
        if (dialog.m_closed) {
            application.openMainMenuScreen();
            return;
        }

        try {
            application.m_state.game = std::make_unique<Game>(dialog.m_options[dialog.m_optionIndex].m_path);
        }
        catch (SaveException&) {
            application.openMainMenuScreen();
            return;
        }

        dialog.m_exit = false;

        application.openOponentNumberScreen();
    };

    openScreen(new Dialog<Save>(*this, "map list screen", Save::findMaps(), onExit));
}

void Application::openSaveListScreen() {
    PN_LOGH2("opening saveListScreen");

    auto onExit = [](Dialog<Save>& dialog, Application& application) {
        if (dialog.m_closed) {
            application.openMainMenuScreen();
            return;
        }

        try {
            application.m_state.game = std::make_unique<Game>(dialog.m_options[dialog.m_optionIndex].m_path);
        }
        catch (SaveException&) {
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

    auto max = m_state.game->maxPlayers();
    for (uint8_t i = 0; i < max; i++)
        oponents.push_back(std::to_string(i));

    auto onExit = [](Dialog<std::string>& dialog, Application& application) {
        if (dialog.m_closed)
            return;

        application.m_state.game->createPlayers(dialog.m_optionIndex);

        application.closeCurrentScreen();  // close self
        application.closeCurrentScreen();  // close map screen

        application.openGameScreen();
    };

    openScreen(new Dialog<std::string>(*this, "oponent number screen", oponents, onExit));
}

void Application::openGameScreen() {
    PN_LOGH2("opening gameScreen");

    PN_LOG_OBJ(m_state.game);

    m_state.game->onLoad();

    openScreen(new GameScreen(*this));
}

void Application::openPauseScreen() {
    PN_LOGH2("opening pauseScreen");

    auto onExit = [](Dialog<std::string>& dialog, Application& application) {
        if (dialog.m_closed)
            return;

        switch (dialog.m_optionIndex) {
            case 0:
                break;

            case 1:
                try {
                    application.m_state.game->save(
                        Save::createSavePath(application.m_state.game->m_mapName));
                }
                catch (SaveException&) {
                    dialog.m_exit = false;
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

    auto winner = m_state.game->getWinner();
    if (winner)
        text = std::string(winner->m_playerName).append(" won");

    auto onExit = [](Dialog<std::string>& dialog, Application& application) {
        application.openMainMenuScreen();
    };

    openScreen(new Dialog<std::string>(*this, text, {"return to main menu"}, onExit, true));
}

void Application::run() {
    while (!m_screens.empty()) {
        if (m_screens.top()->m_exit) {
            closeCurrentScreen();
            continue;
        }

        PN_LOG("showing " << m_screens.top()->m_title);

        m_screens.top()->show();
    }
}
