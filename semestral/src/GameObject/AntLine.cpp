#include "AntLine.h"

#include <ncurses.h>

#include "../Game.h"
#include "../Screen/Screen.h"
#include "Ant.h"

AntLine::AntLine() {
}

AntLine::AntLine(char nestAId, char nestBId) : m_nestAId(nestAId), m_nestBId(nestBId) {
}

void AntLine::switchSide(const AntNest* nest, bool value) {
    if (m_nestA == nest)
        m_nestAActive = value;
    else
        m_nestBActive = value;
}

AntNest* AntLine::otherNest(const AntNest* nest) const {
    if (m_nestA == nest)
        return m_nestB;
    else
        return m_nestA;
}

bool AntLine::otherSideActive(const AntNest* nest) const {
    if (m_nestA == nest)
        return m_nestBActive;
    else
        return m_nestAActive;
}

bool AntLine::friendly() const {
    return m_nestA->player() == m_nestB->player();
}

void AntLine::draw() const {
    attron(COLOR_PAIR(m_color));
    Screen::drawDottedLine(m_nestA->location(), m_nestB->location());
}

void AntLine::update() {
    if (m_nestAActive)
        m_nestA->spawnAnt(m_nestB);

    if (m_nestBActive)
        m_nestB->spawnAnt(m_nestA);
}

void AntLine::onLoad() {
    m_nestA = game()->m_nestMap[m_nestAId];
    m_nestB = game()->m_nestMap[m_nestBId];

    m_nestA->m_lineMap[m_nestBId] = this;
    m_nestB->m_lineMap[m_nestAId] = this;
}

uint8_t AntLine::updatePriority() const {
    return 64;
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
