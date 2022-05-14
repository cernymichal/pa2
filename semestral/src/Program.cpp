#include "Program.h"

#include <ncurses.h>

#include "Ant.h"
#include "Game.h"
#include "Label.h"
#include "log.h"

Program::Program() {
}

void Program::start() {
    // init ncurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    scrollok(stdscr, FALSE);
    curs_set(FALSE);
    start_color();

    init_pair(1, COLOR_WHITE, COLOR_BLACK);

    auto game = Game();

    game.addObject(
        new Label(COLS / 2, LINES / 3, "hello world!", true));

    game.addObject(
        new Ant(18, 3));

    game.addObject(
        new Ant(COLS - 14, LINES - 8));

    game.addObject(
        new Ant(25, LINES - 3));

    game.update();
    game.draw();

    PN_LOG_OBJ(&game);

    timeout(-1);
    getch();

    endwin();
}
