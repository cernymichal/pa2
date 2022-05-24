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
    mvhline(y, x + 1, '-', width - 1);
    mvhline(y + height - 1, x + 1, '-', width - 1);
    mvvline(y + 1, x, '|', height - 2);
    mvvline(y + 1, x + width - 1, '|', height - 2);

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

    for (uint16_t i = 0;; i++, x += dx, y += dy) {
        if (i % 3 == 0)
            mvaddch((uint8_t)y, (uint8_t)x, '.');

        if (abs((uint8_t)x - x2) <= 1 && abs((uint8_t)y - y2) <= 1)
            break;
    }
}
