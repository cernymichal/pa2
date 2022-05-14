#include "Program.h"

#include <ncurses.h>

#include "game_object/Ant.h"
#include "game_object/Label.h"
#include "log.h"
#include "screen/Game.h"

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

    PN_LOG_OBJ(&game);

    game.show();

    endwin();
}
