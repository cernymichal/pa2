#include "Wall.h"

#include <ncurses.h>

Wall::Wall() {
    m_hitDistance = 0;
}

Wall::Wall(uint8_t x, uint8_t y) : GameObject(x, y) {
    m_hitDistance = 0;
}

void Wall::draw() const {
    mvaddch(m_y, m_x, '#' | COLOR_PAIR(m_color));
}

bool Wall::serialize(std::ostream& stream) const {
    return serializeState(stream << "Wall ");
}
