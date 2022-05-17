#include "Screen.h"

#include <ncurses.h>

Screen::Screen(Application& application, const std::string& title)
    : title(title), _application(application) {
}

void Screen::_onExit() {
}

void Screen::show() {
    timeout(_timeoutDelay);

    auto start = std::chrono::steady_clock::now();

    update(std::chrono::nanoseconds(0), ERR);

    while (!exit) {
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
    noecho();
    keypad(stdscr, TRUE);
    scrollok(stdscr, FALSE);
    curs_set(FALSE);
    start_color();

    init_pair(1, COLOR_WHITE, COLOR_BLACK);
}

void Screen::exitNCurses() {
    endwin();
}
