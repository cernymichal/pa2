#pragma once

#include <ncurses.h>

#include <iostream>

class GameObject {
public:
    uint8_t x = 0;
    uint8_t y = 0;
    uint8_t color = COLOR_WHITE;
    uint8_t updatePriority = 128;

    GameObject();

    GameObject(uint8_t x, uint8_t y);

    /**
     * @brief draw game object to the screen with ncurses
     */
    virtual void draw() const;

    /**
     * @brief update game object
     */
    virtual void update();

    /**
     * @brief serialize game object for game saving
     *
     * uses _serialize
     *
     * @param[in] stream output stream
     */
    virtual bool serialize(std::ostream& stream) const = 0;

    /**
     * @brief unserialize game object from save
     *
     * @param[in] stream input stream
     */
    virtual bool unserialize(std::istream& stream);

    /**
     * @brief log debug info
     *
     * @param[in] stream output stream
     */
    virtual std::ostream& log(std::ostream& stream) const;

protected:
    /**
     * @brief serialize game object without type header
     *
     * @param[in] stream output stream
     */
    virtual bool _serialize(std::ostream& stream) const;
};