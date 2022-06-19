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
    /**
     * @param[in] application
     * @param[in] title
     * @param[in] onExit function to be called after exit
     * @param[in] options vector<T> of dialog options, with c_str()
     * @param[in] showTitle
     */
    Dialog(Application& application, const std::string& title, const std::vector<T>& options,
           std::function<void(Dialog<T>&, Application&)> onExit, bool showTitle = false)
        : Screen(application, title), m_options(options), m_showTitle(showTitle), m_onExitF(onExit) {
    }

    bool closed() const {
        return m_closed;
    }

    uint8_t selectedIndex() const {
        return m_optionIndex;
    }

    const T& selectedOption() const {
        return m_options[m_optionIndex];
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
                if (m_optionIndex > 0)
                    m_optionIndex--;
                break;

            case KEY_DOWN:
            case 's':
            case 'S':
                if (m_optionIndex < m_options.size() - 1)
                    m_optionIndex++;
                break;

            case ';':
                m_closed = true;
            case KEY_ENTER:
            case '\n':
            case ' ':
                m_exit = true;
                break;
        }

        draw();
    }

    virtual void reset() override {
        Screen::reset();
        m_closed = false;
    }

protected:
    std::vector<T> m_options;
    bool m_showTitle;
    uint8_t m_optionIndex = 0;
    bool m_closed = false;

    /**
     * @brief call _onExifF after exit
     */
    virtual void onExit() override {
        m_onExitF(*this, m_application);
    }

private:
    std::function<void(Dialog<T>&, Application&)> m_onExitF;

    /**
     * @brief draw dialog to the screen
     */
    void draw() const {
        attrclr();
        clear();

        uint8_t offset = 1;

        if (m_showTitle) {
            attron(A_UNDERLINE);
            mvaddstr(1, 2, m_title.c_str());
            attroff(A_UNDERLINE);
            offset = 3;
        }

        for (size_t i = 0; i < m_options.size(); i++)
            mvaddstr(i + offset, 3, m_options[i].c_str());

        if (m_options.size() > 0)
            mvaddch(m_optionIndex + offset, 2, '>');

        refresh();
    }
};
