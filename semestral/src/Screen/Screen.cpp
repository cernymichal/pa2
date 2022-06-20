#include "Screen.h"

#include <ncurses.h>

Screen::Screen(Application& application, const std::string& title)
    : m_title(title), m_application(application) {
}

Screen::~Screen() {
}

bool Screen::exited() const {
    return m_exit;
}

void Screen::onExit() {
}

void Screen::show() {
    using namespace std::literals::chrono_literals;

    timeout(m_timeoutDelay);

    auto start = std::chrono::steady_clock::now();

    // call first update for drawing
    update(0ns, ERR);

    while (!m_exit) {
        // calculate delta time from last update
        auto newStart = std::chrono::steady_clock::now();
        auto dt = newStart - start;
        start = newStart;

        update(dt, getch());
    }

    onExit();
}

void Screen::reset() {
    m_exit = false;
}

void Screen::initNCurses() {
    initscr();
    cbreak();
    noecho();                 // dont echo written characters
    keypad(stdscr, TRUE);     // catch keypad input
    scrollok(stdscr, FALSE);  // dont scroll on new line
    curs_set(FALSE);          // invisible cursor
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

void Screen::drawBox(const Vector2<uint8_t>& location, const Vector2<uint8_t>& size) {
    // sides
    mvhline(location.y, location.x + 1, '-', size.x - 1);
    mvhline(location.y + size.y - 1, location.x + 1, '-', size.x - 1);
    mvvline(location.y + 1, location.x, '|', size.y - 2);
    mvvline(location.y + 1, location.x + size.x - 1, '|', size.y - 2);

    // corners
    mvaddch(location.y, location.x, '+');
    mvaddch(location.y + size.y - 1, location.x, '+');
    mvaddch(location.y + size.y - 1, location.x + size.x - 1, '+');
    mvaddch(location.y, location.x + size.x - 1, '+');
}

void Screen::drawDottedLine(const Vector2<uint8_t>& a, const Vector2<uint8_t>& b) {
    Vector2<double> delta = b - a;
    delta /= delta.length();

    Vector2<double> position = a;

    // walk on line leaving dots every 3 steps
    for (uint16_t i = 0;; i++, position += delta) {
        if (i % 3 == 0)
            mvaddch((uint8_t)position.y, (uint8_t)position.x, '.');

        if (abs((uint8_t)position.x - b.x) <= 1 && abs((uint8_t)position.y - b.y) <= 1)
            break;
    }
}
