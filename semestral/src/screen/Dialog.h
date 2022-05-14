#pragma once

#include <string>
#include <vector>

#include "Screen.h"

/**
 * dialog screen with blocking input
 * can be closed without choosing with ESC
 * otherwise chosen option is in optionIndex and closed is false
 */
class Dialog : public Screen {
    std::vector<std::string> _options;

public:
    uint8_t optionIndex = 0;
    bool closed = false;

    Dialog(const std::vector<std::string>& options);

    /**
     * @brief move cursor or select option
     *
     * @param[in] dt delta time since last update
     * @param[in] key ncurses input key
     */
    virtual void update(double dt, int key) override;
};
