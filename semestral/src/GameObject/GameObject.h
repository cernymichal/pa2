#pragma once

#include <iostream>

#include "../Screen/Screen.h"
#include "../utils/Vector2.cpp"

class Game;

/**
 * @brief abstract root of objects represented in Game
 *
 * has location, color, destroyed (should be removed from Game?), collision distance and update priority
 */
class GameObject {
public:
    uint8_t m_color = COLOR_PAIR_WHITE;
    int16_t m_hitDistance = -1;  // dont check collision by default

    explicit GameObject();

    explicit GameObject(const Vector2<uint8_t>& location);

    virtual ~GameObject();

    const Vector2<uint8_t>& location() const;

    bool destroyed() const;

    void destroy();

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
    virtual void onAdd(Game* game);

    /**
     * @brief called after game is loaded
     */
    virtual void onLoad();

    virtual void onErase();

    bool collisionEnabled() const;

    /**
     * @brief handle collision with other GameObject
     */
    virtual void collideWith(GameObject& object);

    /**
     * @returns update priority of object
     */
    virtual uint8_t updatePriority() const;

    /**
     * @returns demangled class name
     */
    std::string name() const;

    /**
     * @brief prefix serialization with class name
     *
     * uses serializeState
     *
     * @param[in] stream output stream
     */
    bool serialize(std::ostream& stream) const;

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
    Vector2<uint8_t> m_location = {0, 0};
    bool m_collision = false;

    Game* game() const;

    /**
     * @brief serialize for game saving
     *
     * format: "{m_x}m_y{y} {m_color} {m_destroyed} {m_hitDistance}"
     *
     * @param[in] stream output stream
     */
    virtual bool serializeState(std::ostream& stream) const;

private:
    bool m_destroyed = false;
    Game* m_game = nullptr;
};
