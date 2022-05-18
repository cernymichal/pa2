#include "Dialog.h"

#include <ncurses.h>

#include "../Save.h"

template <typename T>
Dialog<T>::Dialog(Application& application, const std::string& title, const std::vector<T>& options,
                  std::function<void(Dialog<T>&, Application&)> onExit, bool showTitle)
    : Screen(application, title), options(options), showTitle(showTitle), _onExitF(onExit) {
}

template <typename T>
void Dialog<T>::update(std::chrono::nanoseconds dt, int key) {
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
            if (optionIndex < options.size() - 1)
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

template <typename T>
void Dialog<T>::_onExit() {
    _onExitF(*this, _application);
}

template <typename T>
void Dialog<T>::_draw() const {
    clear();

    uint8_t offset = 1;

    if (showTitle) {
        mvaddstr(1, 2, title.c_str());
        offset = 3;
    }

    for (size_t i = 0; i < options.size(); i++)
        mvaddstr(i + offset, 3, options[i].c_str());

    if (options.size() > 0)
        mvaddch(optionIndex + offset, 2, '>');

    refresh();
}

template class Dialog<std::string>;
template class Dialog<Save>;
