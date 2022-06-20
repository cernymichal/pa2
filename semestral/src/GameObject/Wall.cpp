#include "Wall.h"

#include <ncurses.h>

Wall::Wall() {
    m_hitDistance = 0;
}

Wall::Wall(const Vector2<uint8_t>& location) : GameObject(location) {
    m_hitDistance = 0;
}

void Wall::draw() const {
    mvaddch(m_location.y, m_location.x, '#' | COLOR_PAIR(m_color));
}

bool Wall::serialize(std::ostream& stream) const {
    return serializeState(stream << "Wall ");
}
