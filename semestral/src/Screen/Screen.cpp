#include "Screen.h"

#include <ncurses.h>

#include "../utils.cpp"

Screen::Screen(Application& application, const std::string& title)
    : title(title), _application(application) {
}

void Screen::_onExit() {
}

void Screen::show() {
    timeout(_timeoutDelay);

    auto start = std::chrono::steady_clock::now();

    // call first update for drawing
    update(std::chrono::nanoseconds(0), ERR);

    while (!exit) {
        // calculate delta time from last update
        auto newStart = std::chrono::steady_clock::now();
        auto dt = newStart - start;
        start = newStart;

        update(dt, getch());
    }

    _onExit();
}

void Screen::initNCurses() {
    initscr();
    cbreak();
    noecho(); // dont echo written characters
    keypad(stdscr, TRUE); // catch keypad input
    scrollok(stdscr, FALSE); // dont scroll on new line
    curs_set(FALSE); // invisible cursor
    start_color();

    // init color pairs
    init_pair(COLOR_PAIR_WHITE, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_PAIR_BLUE, COLOR_BLUE, COLOR_BLACK);
    init_pair(COLOR_PAIR_RED, COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_PAIR_GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_PAIR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(COLOR_PAIR_CYAN, COLOR_CYAN, COLOR_BLACK);
    init_pair(COLOR_PAIR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_PAIR_BLACK, COLOR_BLACK, COLOR_WHITE);
}

void Screen::exitNCurses() {
    endwin();
}

void Screen::drawBox(uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
    // sides
    mvhline(y, x + 1, '-', width - 1);
    mvhline(y + height - 1, x + 1, '-', width - 1);
    mvvline(y + 1, x, '|', height - 2);
    mvvline(y + 1, x + width - 1, '|', height - 2);

    // corners
    mvaddch(y, x, '+');
    mvaddch(y + height - 1, x, '+');
    mvaddch(y + height - 1, x + width - 1, '+');
    mvaddch(y, x + width - 1, '+');
}

void Screen::drawDottedLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    float d = sqrt(dx * dx + dy * dy);
    dx /= d;
    dy /= d;

    float x = (int)x1;
    float y = (int)y1;

    // walk on line leaving dots every 3 steps
    for (uint16_t i = 0;; i++, x += dx, y += dy) {
        if (i % 3 == 0)
            mvaddch((uint8_t)y, (uint8_t)x, '.');

        if (abs((uint8_t)x - x2) <= 1 && abs((uint8_t)y - y2) <= 1)
            break;
    }
}
