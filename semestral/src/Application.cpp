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

    GameBuilder::initGameObjectInstatiators();
    Screen::initNCurses();

    openMainMenuScreen();
    run();

    Screen::exitNCurses();
}

void Application::closeCurrentScreen() {
    m_screens.pop();
}

void Application::openMainMenuScreen() {
    PN_LOGH2("opening mainMenuScreen");

    m_state = ApplicationState();

    auto onExit = [](Dialog<std::string>& dialog, Application& application) {
        if (dialog.closed())
            return;

        switch (dialog.selectedIndex()) {
            case 0:
                if (Save::emptyMapDirectory()) {
                    PN_LOG("no maps found");

                    dialog.reset();

                    break;
                }
                application.openMapListScreen();
                break;

            case 1:
                if (Save::emptySaveDirectory()) {
                    PN_LOG("no saves found");

                    dialog.reset();

                    break;
                }
                application.openSaveListScreen();
                break;
        }
    };

    openScreen<Dialog<std::string>>(
        *this, "pins & needles",
        std::vector<std::string>{"new game",
                                 "load",
                                 "exit"},
        onExit,
        true);
}

void Application::openMapListScreen() {
    PN_LOGH2("opening mapListScreen");

    auto onExit = [](Dialog<Save>& dialog, Application& application) {
        if (dialog.closed()) {
            application.openMainMenuScreen();
            return;
        }

        try {
            application.m_state.gameBuilder.loadFromFile(dialog.selectedOption().path());
        }
        catch (SaveException&) {
            application.openMainMenuScreen();
            return;
        }

        dialog.reset();

        application.openOponentNumberScreen();
    };

    openScreen<Dialog<Save>>(*this, "map list screen", Save::findMaps(), onExit);
}

void Application::openSaveListScreen() {
    PN_LOGH2("opening saveListScreen");

    auto onExit = [](Dialog<Save>& dialog, Application& application) {
        if (dialog.closed()) {
            application.openMainMenuScreen();
            return;
        }

        try {
            application.m_state.gameBuilder.loadFromFile(dialog.selectedOption().path());
        }
        catch (SaveException&) {
            application.openMainMenuScreen();
            return;
        }

        application.openGameScreen();
    };

    openScreen<SaveList>(*this, Save::findSaves(), onExit);
}

void Application::openOponentNumberScreen() {
    PN_LOGH2("opening oponentNumberScreen");

    auto oponents = std::vector<std::string>();

    auto max = m_state.gameBuilder.maxPlayers();
    for (uint8_t i = 0; i < max; i++)
        oponents.push_back(std::to_string(i));

    auto onExit = [](Dialog<std::string>& dialog, Application& application) {
        if (dialog.closed())
            return;

        application.m_state.gameBuilder.createPlayers(dialog.selectedIndex());

        application.closeCurrentScreen();  // close self
        application.closeCurrentScreen();  // close map screen

        application.openGameScreen();
    };

    openScreen<Dialog<std::string>>(*this, "oponent number screen", oponents, onExit);
}

void Application::openGameScreen() {
    PN_LOGH2("opening gameScreen");

    PN_LOG("getting game from builder");
    m_state.game = m_state.gameBuilder.getGame();

    PN_LOG_OBJ(m_state.game);

    m_state.game->onLoad();

    openScreen<GameScreen>(*this);
}

void Application::openPauseScreen() {
    PN_LOGH2("opening pauseScreen");

    auto onExit = [](Dialog<std::string>& dialog, Application& application) {
        if (dialog.closed())
            return;

        switch (dialog.selectedIndex()) {
            case 0:
                break;

            case 1:
                try {
                    GameBuilder::saveToFile(
                        *application.m_state.game,
                        Save::createSavePath(application.m_state.game->mapName()));
                }
                catch (SaveException&) {
                    dialog.reset();
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

    openScreen<Dialog<std::string>>(
        *this, "pause screen",
        std::vector<std::string>{"continue",
                                 "save",
                                 "surrender",
                                 "return to main menu"},
        onExit);
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

    openScreen<Dialog<std::string>>(*this, text, std::vector<std::string>{"return to main menu"}, onExit, true);
}

void Application::run() {
    while (!m_screens.empty()) {
        if (m_screens.top()->exited()) {
            closeCurrentScreen();
            continue;
        }

        PN_LOG("showing " << m_screens.top()->m_title);

        m_screens.top()->show();
    }
}
