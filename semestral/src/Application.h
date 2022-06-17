#pragma once

#include <memory>
#include <stack>

#include "Game.h"
#include "Screen/Screen.h"

/**
 * @brief holds allocated Game
 */
struct ApplicationState {
    std::unique_ptr<Game> game;
};

/**
 * @brief manages Screens and holds ApplicationState
 */
class Application {
public:
    ApplicationState m_state;

    Application();

    /**
     * @brief open main menu and run the main program loop
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

    /**
     * @brief pops screen of the screen stack
     */
    void closeCurrentScreen();

    /**
     * resets state
     */
    void openMainMenuScreen();

    /**
     * @brief opens a dialog of map in default map dir
     */
    void openMapListScreen();

    /**
     * @brief opens a dialog of saves in default save dir
     */
    void openSaveListScreen();

    /**
     * - reads maximum nuber of players from game
     * - expects state.game not to be null
     */
    void openOponentNumberScreen();

    /**
     * - calls onLoad in game
     * - expects state.game not to be null
     */
    void openGameScreen();

    /**
     * - expects state.game not to be null
     */
    void openPauseScreen();

    /**
     * - expects state.game not to be null
     */
    void openResultsScreen();

private:
    std::stack<std::unique_ptr<Screen>> m_screens;

    /**
     * @brief main program loop
     *
     * shows top screen
     */
    void run();
};
