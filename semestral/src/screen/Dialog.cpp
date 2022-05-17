#include "Dialog.h"

#include <ncurses.h>

Dialog::Dialog(Application& application, const std::string& title, std::function<void(Dialog&, Application&)> onExit,
               const std::vector<std::string>& options, bool showTitle)
    : Screen(application, title), _onExitF(onExit), _options(options), _showTitle(showTitle) {
}

void Dialog::update(std::chrono::nanoseconds dt, int key) {
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

void Dialog::_onExit() {
    _onExitF(*this, _application);
}

void Dialog::_draw() const {
    clear();

    uint8_t offset = 1;

    if (_showTitle) {
        mvaddstr(1, 2, title.c_str());
        offset = 3;
    }

    for (size_t i = 0; i < _options.size(); i++)
        mvaddstr(i + offset, 3, _options[i].c_str());

    if (_options.size() > 0)
        mvaddch(optionIndex + offset, 2, '>');

    refresh();
}
