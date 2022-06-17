#include "Ant.h"

#include <ncurses.h>

#include "../log.h"
#include "../utils.cpp"
#include "AntNest.h"

Ant::Ant() {
    m_hitDistance = 0;
}

Ant::Ant(uint8_t x, uint8_t y, Player* player, int8_t tx, int8_t ty) : PlayerUnit(x, y, player), m_tx(tx), m_ty(ty) {
    m_hitDistance = 0;
    m_mvx = x;
    m_mvy = y;
}

void Ant::draw() const {
    mvaddch(m_y, m_x, 'm' | COLOR_PAIR(m_color));
}

void Ant::update() {
    if (m_x != m_tx || m_y != m_ty) {
        float dx = m_tx - m_x;
        float dy = m_ty - m_y;
        float d = sqrt(dx * dx + dy * dy);
        dx /= d;
        dy /= d;

        m_mvx += dx;
        m_mvy += dy;

        m_x = m_mvx;
        m_y = m_mvy;
    }
}

void Ant::collideWith(GameObject& object) {
    if (m_dead)
        return;

    auto ant = dynamic_cast<Ant*>(&object);

    if (ant && ant->player() != player()) {
        m_dead = true;
        return;
    }

    auto nest = dynamic_cast<AntNest*>(&object);

    if (nest && nest->m_x == m_tx && nest->m_y == m_ty)
        m_dead = true;
}

bool Ant::serialize(std::ostream& stream) const {
    return serializeState(stream << "Ant ");
}

bool Ant::serializeState(std::ostream& stream) const {
    stream << static_cast<short>(m_tx)
           << ' ' << static_cast<short>(m_ty)
           << ' ' << m_mvx
           << ' ' << m_mvy
           << ' ';

    return PlayerUnit::serializeState(stream);
}

bool Ant::unserialize(std::istream& stream) {
    short temp;

    stream >> temp;
    m_tx = temp;

    stream >> temp;
    m_ty = temp;

    stream >> m_mvx >> m_mvy;

    return PlayerUnit::unserialize(stream);
}
