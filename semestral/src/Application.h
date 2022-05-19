#pragma once

#include <memory>
#include <stack>

#include "Game.h"
#include "Screen/Screen.h"

struct ApplicationState {
    std::unique_ptr<Game> game;
};

class Application {
public:
    ApplicationState state;

    Application();

    /**
     * open main menu and run the main program loop
     */
    void start();

    /**
     * @brief add screen on top of the screen stack
     *
     * pops last screen if set to exit
     *
     * @param[in] screen pointer to Screen, will be managed by Application
     */
    void openScreen(Screen* screen);

    void closeCurrentScreen();

    void openMainMenuScreen();

    void openMapListScreen();

    void openSaveListScreen();

    void openOponentNumberScreen();

    void openGameScreen();

    void openPauseScreen();

    void openResultsScreen();

private:
    std::stack<std::unique_ptr<Screen>> _screens;

    /**
     * @brief main program loop
     *
     * shows top screen
     */
    void _run();
};
