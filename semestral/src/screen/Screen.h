#pragma once

class Screen {
protected:
    int timeoutDelay = -1;

public:
    bool exit = false;

    /**
     * @brief update screen
     *
     * @param[in] dt delta time since last update
     * @param[in] key ncurses input key
     */
    virtual void update(double dt, int key) = 0;

    /**
     * @brief check for input and call update
     */
    void show();
};
