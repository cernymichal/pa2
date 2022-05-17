#pragma once

#include <functional>
#include <string>
#include <vector>

#include "Screen.h"

/**
 * dialog screen with blocking input
 * can be closed without choosing with BACKSPACE
 * otherwise chosen option is in optionIndex and closed is false
 */
class Dialog : public Screen {
public:
    uint8_t optionIndex = 0;
    bool closed = false;

    Dialog(Application& application, const std::string& title, std::function<void(Dialog&, Application&)> onExit,
           const std::vector<std::string>& options, bool showTitle = false);

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
    std::function<void(Dialog&, Application&)> _onExitF;
    const std::vector<std::string> _options;
    bool _showTitle;

    void _draw() const;
};
