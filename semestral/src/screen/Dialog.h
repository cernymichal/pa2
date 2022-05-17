#pragma once

#include <functional>
#include <string>
#include <vector>

#include "Screen.h"

// TODO docs
/**
 * dialog screen with blocking input
 * can be closed without choosing with BACKSPACE
 * otherwise chosen option is in optionIndex and closed is false
 */
template <typename T>
class Dialog : public Screen {
public:
    std::function<void(Dialog<T>&, Application&)> onExitF;
    const std::vector<T> options;
    bool showTitle;
    uint8_t optionIndex = 0;
    bool closed = false;

    Dialog(Application& application, const std::string& title, std::function<void(Dialog<T>&, Application&)> onExit,
           const std::vector<T>& options, bool showTitle = false);

    /**
     * @brief move cursor or select option
     *
     * @param[in] dt delta time since last update
     * @param[in] key ncurses input key
     */
    virtual void update(std::chrono::nanoseconds dt, int key) override;

protected:
    virtual void _onExit() override;

private:
    void _draw() const;
};
