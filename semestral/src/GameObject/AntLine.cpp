#include "AntLine.h"

#include <ncurses.h>

#include "../Game.h"
#include "../Screen/Screen.h"
#include "Ant.h"

AntLine::AntLine() {
}

AntLine::AntLine(char nestAId, char nestBId) : m_nestAId(nestAId), m_nestBId(nestBId) {
}

void AntLine::switchSide(AntNest* nest, bool value) {
    if (m_nestA == nest)
        m_nestAActive = value;
    else
        m_nestBActive = value;
}

AntNest* AntLine::otherNest(AntNest* nest) {
    if (m_nestA == nest)
        return m_nestB;
    else
        return m_nestA;
}

bool AntLine::otherSideActive(AntNest* nest) {
    if (m_nestA == nest)
        return m_nestBActive;
    else
        return m_nestAActive;
}

bool AntLine::friendly() {
    return m_nestA->player() == m_nestB->player();
}

void AntLine::draw() const {
    attron(COLOR_PAIR(m_color));
    Screen::drawDottedLine(m_nestA->m_x, m_nestA->m_y, m_nestB->m_x, m_nestB->m_y);
}

void AntLine::update() {
    if (m_nestAActive && m_nestA->m_ants > 0) {
        m_nestA->m_ants--;
        m_game->addObject<Ant>(m_nestA->m_x, m_nestA->m_y, m_nestA->player(), m_nestB->m_x, m_nestB->m_y);
    }

    if (m_nestBActive && m_nestB->m_ants > 0) {
        m_nestB->m_ants--;
        m_game->addObject<Ant>(m_nestB->m_x, m_nestB->m_y, m_nestB->player(), m_nestA->m_x, m_nestA->m_y);
    }
}

void AntLine::onLoad() {
    m_nestA = m_game->m_nestMap[m_nestAId];
    m_nestB = m_game->m_nestMap[m_nestBId];

    m_nestA->m_lineMap[m_nestBId] = this;
    m_nestB->m_lineMap[m_nestAId] = this;
}

uint8_t AntLine::updatePriority() const {
    return 64;
}

bool AntLine::serialize(std::ostream& stream) const {
    return serializeState(stream << "AntLine ");
}

bool AntLine::serializeState(std::ostream& stream) const {
    stream << m_nestAId
           << ' ' << m_nestAActive
           << ' ' << m_nestBId
           << ' ' << m_nestBActive
           << ' ';

    return GameObject::serializeState(stream);
}

bool AntLine::unserialize(std::istream& stream) {
    stream >> m_nestAId >> m_nestAActive >> m_nestBId >> m_nestBActive;

    return GameObject::unserialize(stream);
}
