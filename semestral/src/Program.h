#pragma once

class Program {
public:
    Program();

    /**
     * @brief main program loop
     */
    void start();

private:
    /**
     * @brief show main menu screen
     */
    void _mainMenuScreen();

    void _mapListScreen();

    void _saveListScreen();

    void _oponentNumberScreen();

    void _gameScreen();

    void _pauseScreen();

    void _resultsScreen();
};
