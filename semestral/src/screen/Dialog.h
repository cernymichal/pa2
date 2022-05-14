#pragma once

#include <string>
#include <vector>

#include "Screen.h"

/**
 * dialog screen with blocking input
 * can be closed without choosing with BACKSPACE
 * otherwise chosen option is in optionIndex and closed is false
 */
class Dialog : public Screen {
    const std::vector<std::string> _options;
    const std::string _header;

public:
    uint8_t optionIndex = 0;
    bool closed = false;

    Dialog(const std::vector<std::string>& options, const std::string& header = "");

    /**
     * @brief move cursor or select option
     *
     * @param[in] dt delta time since last update
     * @param[in] key ncurses input key
     */
    virtual void update(int64_t dt, int key) override;

private:
    void _draw() const;
};
