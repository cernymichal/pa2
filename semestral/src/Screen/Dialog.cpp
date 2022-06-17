#pragma once

#include <ncurses.h>

#include <functional>
#include <string>
#include <vector>

#include "Screen.h"

/**
 * @brief screen with options to choose from
 *
 * - input blocking
 * - can be closed without choosing with BACKSPACE
 * - otherwise chosen option is in optionIndex and closed is false
 */
template <typename T>
class Dialog : public Screen {
public:
    std::vector<T> options;
    bool showTitle;
    uint8_t optionIndex = 0;
    bool closed = false;

    /**
     * @param[in] application
     * @param[in] title
     * @param[in] onExit function to be called after exit
     * @param[in] options vector<T> of dialog options, with c_str()
     * @param[in] showTitle
     */
    Dialog(Application& application, const std::string& title, const std::vector<T>& options,
           std::function<void(Dialog<T>&, Application&)> onExit, bool showTitle = false)
        : Screen(application, title), options(options), showTitle(showTitle), _onExitF(onExit) {
    }

    /**
     * @brief move cursor or select option
     *
     * @param[in] dt delta time since last update
     * @param[in] key ncurses input key
     */
    virtual void update(std::chrono::nanoseconds dt, int key) override {
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

            case ';':
                closed = true;
            case KEY_ENTER:
            case '\n':
            case ' ':
                exit = true;
                break;
        }

        _draw();
    }

protected:
    /**
     * @brief call _onExifF after exit
     */
    virtual void _onExit() override {
        _onExitF(*this, _application);
    }

private:
    std::function<void(Dialog<T>&, Application&)> _onExitF;

    /**
     * @brief draw dialog to the screen
     */
    void _draw() const {
        attrclr();
        clear();

        uint8_t offset = 1;

        if (showTitle) {
            attron(A_UNDERLINE);
            mvaddstr(1, 2, title.c_str());
            attroff(A_UNDERLINE);
            offset = 3;
        }

        for (size_t i = 0; i < options.size(); i++)
            mvaddstr(i + offset, 3, options[i].c_str());

        if (options.size() > 0)
            mvaddch(optionIndex + offset, 2, '>');

        refresh();
    }
};
