#pragma once

#include <chrono>
#include <cstdint>
#include <string>

#include "../log.h"
#include "../utils.cpp"

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
 * - m_timeoutDelay - input wait
 */
class Screen {
public:
    std::string m_title;

    Screen(Application& application, const std::string& title = "untitled screen");

    virtual ~Screen();

    bool exited() const;

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
     * @brief reset exit state
     */
    virtual void reset();

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
    static void drawBox(const Vector2<uint8_t>& location, const Vector2<uint8_t>& size);

    /**
     * @brief draw dotted line between points to default screen
     */
    static void drawDottedLine(const Vector2<uint8_t>& a, const Vector2<uint8_t>& b);

protected:
    bool m_exit = false;
    Application& m_application;
    int m_timeoutDelay = -1;

    /**
     * @brief called from update after exit
     */
    virtual void onExit();
};
