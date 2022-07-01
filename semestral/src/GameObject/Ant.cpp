#include "Ant.h"

#include <ncurses.h>

#include "../utils/log.h"
#include "AntNest.h"

Ant::Ant() {
    m_collision = true;
}

Ant::Ant(const Vector2<uint8_t>& location, Player* player, Vector2<uint8_t> targetLocation)
    : PlayerUnit(location, player), m_targetLocation(targetLocation) {
    m_collision = true;
    m_mvLocation = m_location;
}

const Vector2<uint8_t>& Ant::target() const {
    return m_targetLocation;
}

void Ant::draw() const {
    mvaddch(m_location.y, m_location.x, 'm' | COLOR_PAIR(m_color));
}

void Ant::update() {
    if (m_location != m_targetLocation) {
        Vector2<double> delta = m_targetLocation - m_mvLocation;
        delta /= delta.length();

        m_mvLocation += delta;

        m_location = m_mvLocation;
    }
}

void Ant::collideWith(GameObject& object) {
    if (destroyed())
        return;

    auto ant = dynamic_cast<Ant*>(&object);

    if (ant && ant->player() != player() && !ant->destroyed()) {
        destroy();
        ant->destroy();
    }
}

bool Ant::serializeState(std::ostream& stream) const {
    stream << static_cast<short>(m_targetLocation.x)
           << ' ' << static_cast<short>(m_targetLocation.y)
           << ' ' << m_mvLocation.x
           << ' ' << m_mvLocation.y
           << ' ';

    return PlayerUnit::serializeState(stream);
}

bool Ant::unserialize(std::istream& stream) {
    short temp;

    stream >> temp;
    m_targetLocation.x = temp;

    stream >> temp;
    m_targetLocation.y = temp;

    stream >> m_mvLocation.x >> m_mvLocation.y;

    return PlayerUnit::unserialize(stream);
}
