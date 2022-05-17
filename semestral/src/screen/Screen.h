#pragma once

#include <chrono>
#include <cstdint>
#include <string>

#include "../log.h"

class Application;

class Screen {
public:
    std::string title;
    bool exit = false;

    Screen(Application& application, const std::string& title = "untitled screen");

    /**
     * @brief update screen
     *
     * @param[in] dt delta time since last update
     * @param[in] key ncurses input key
     */
    virtual void update(std::chrono::nanoseconds dt, int key) = 0;

    /**
     * @brief check for input and call update
     */
    void show();

    static void initNCurses();

    static void exitNCurses();

protected:
    Application& _application;
    int _timeoutDelay = -1;

    virtual void _onExit();
};
