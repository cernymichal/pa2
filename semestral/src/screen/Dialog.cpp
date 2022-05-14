#include "Dialog.h"

#include <ncurses.h>

Dialog::Dialog(const std::vector<std::string>& options, const std::string& header) : _options(options), _header(header) {
}

void Dialog::update(int64_t dt, int key) {
    switch (key) {
        case KEY_UP:
        case 'w':
        case 'W':
            if (optionIndex > 0)
                optionIndex--;
            break;

        case KEY_DOWN:
        case 's':
        case 'S':
            if (optionIndex < _options.size() - 1)
                optionIndex++;
            break;

        case KEY_BACKSPACE:
            closed = true;
        case KEY_ENTER:
        case '\n':
        case ' ':
            exit = true;
            break;
    }

    _draw();
}

void Dialog::_draw() const {
    clear();

    uint8_t offset = 1;

    if (_header.size() > 0) {
        mvaddstr(1, 2, _header.c_str());
        offset = 3;
    }

    for (size_t i = 0; i < _options.size(); i++)
        mvaddstr(i + offset, 3, _options[i].c_str());

    if (_options.size() > 0)
        mvaddch(optionIndex + offset, 2, '>');

    refresh();
}
