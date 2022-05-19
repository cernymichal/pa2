#pragma once

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
           std::function<void(Dialog<T>&, Application&)> onExit, bool showTitle = false);

    /**
     * @brief move cursor or select option
     *
     * @param[in] dt delta time since last update
     * @param[in] key ncurses input key
     */
    virtual void update(std::chrono::nanoseconds dt, int key) override;

protected:
    /**
     * @brief call _onExifF after exit
     */
    virtual void _onExit() override;

private:
    std::function<void(Dialog<T>&, Application&)> _onExitF;

    /**
     * @brief draw dialog to the screen
     */
    void _draw() const;
};
