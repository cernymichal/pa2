#pragma once

#include <chrono>
#include <cstdint>
#include <string>

#include "../log.h"

#define COLOR_PAIR_WHITE 0
#define COLOR_PAIR_BLUE 1
#define COLOR_PAIR_RED 2
#define COLOR_PAIR_GREEN 3
#define COLOR_PAIR_MAGENTA 4
#define COLOR_PAIR_CYAN 5
#define COLOR_PAIR_YELLOW 6
#define COLOR_PAIR_BLACK 7

/**
 * @brief set screen attributes to default
 */
#define attrclr() attrset(A_NORMAL | COLOR_PAIR(COLOR_PAIR_WHITE))

class Application;

/**
 * @brief handle user input and drawing
 *
 * - _timeoutDelay - input wait
 */
class Screen {
public:
    std::string title;
    bool exit = false;

    Screen(Application& application, const std::string& title = "untitled screen");

    /**
     * @brief update screen and handle input
     *
     * @param[in] dt delta time since last update
     * @param[in] key ncurses input key
     */
    virtual void update(std::chrono::nanoseconds dt, int key) = 0;

    /**
     * @brief catch input and call update
     */
    void show();

    /**
     * @brief initialize ncurses
     */
    static void initNCurses();

    /**
     * @brief exit ncurses
     */
    static void exitNCurses();

    /**
     * @brief draw a box to default screen
     */
    static void drawBox(uint8_t x, uint8_t y, uint8_t width, uint8_t height);

    /**
     * @brief draw dotted line between points to default screen
     */
    static void drawDottedLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

protected:
    Application& _application;
    int _timeoutDelay = -1;

    /**
     * @brief called from update after exit
     */
    virtual void _onExit();
};
