#pragma once

#include <ncurses.h>

#include <iostream>

class GameObject {
public:
    uint8_t x = 0;
    uint8_t y = 0;
    uint8_t color = COLOR_WHITE;
    uint8_t updatePriority = 128;

    GameObject(uint8_t x, uint8_t y);

    /**
     * @brief draw game object to the screen with ncurses
     */
    virtual void draw() const;

    /**
     * @brief update game object every tick
     */
    virtual void update();

    /**
     * @brief serialize game object for game saving
     *
     * @param[in] stream output stream
     */
    virtual void serialize(std::ostream &stream) const = 0;
};
