#include "Screen.h"

#include <ncurses.h>

void Screen::show() {
    // todo

    update(0, ERR);

    timeout(timeoutDelay);
    update(1, getch());
}
