#pragma once

#include <iostream>

#include "../Screen/Screen.h"

class Game;

/**
 * @brief abstract root of objects represented in Game
 * 
 * has location, color, dead (should be removed from Game?), collision distance and update priority
 */
class GameObject {
public:
    uint8_t x = 0;
    uint8_t y = 0;
    uint8_t color = COLOR_PAIR_WHITE;
    bool dead = false;
    int16_t hitDistance = -1; // dont check collision by default
    uint8_t updatePriority = 128;

    Game* _game = nullptr;

    GameObject();

    GameObject(uint8_t x, uint8_t y);

    /**
     * @brief draw to the screen with ncurses
     */
    virtual void draw() const;

    /**
     * @brief update game object
     */
    virtual void update();

    /**
     * @brief called after object is added to game
     */
    virtual void onAdd();

    /**
     * @brief called after game is loaded
     */
    virtual void onLoad();

    /**
     * @brief handle collision with other GameObject
     */
    virtual void collideWith(GameObject& object);

    /**
     * @brief serialize for game saving
     *
     * uses _serialize
     *
     * @param[in] stream output stream
     */
    virtual bool serialize(std::ostream& stream) const = 0;

    /**
     * @brief unserialize from save
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
     * @brief serialize without type header
     * 
     * format: "{x} {y} {color} {dead} {hitDistance} {updatePriority}"
     *
     * @param[in] stream output stream
     */
    virtual bool _serialize(std::ostream& stream) const;
};
