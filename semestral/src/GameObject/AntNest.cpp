#include "AntNest.h"

#include <ncurses.h>

#include "../Game.h"
#include "../Screen/Screen.h"
#include "Ant.h"
#include "AntLine.h"

#define SPAWN_PERIOD 3
#define NEUTRAL_DEFENDERS 10

AntNest::AntNest() {
    m_hitDistance = 1;
}

AntNest::AntNest(uint8_t x, uint8_t y, char id, bool starting) : PlayerUnit(x, y), m_nestId(id), m_starting(starting) {
    m_hitDistance = 1;

    if (!starting)
        m_ants = NEUTRAL_DEFENDERS;
}

void AntNest::disableLines() {
    for (auto& line : m_lineMap)
        line.second->switchSide(this, false);
}

bool AntNest::contested() {
    for (const auto& line : m_lineMap) {
        if (line.second->otherSideActive(this) && !line.second->friendly())
            return true;
    }

    return false;
}

void AntNest::draw() const {
    attron(COLOR_PAIR(m_color));
    attron(A_BOLD);

    mvaddch(m_y, m_x - 1, m_nestId);

    attroff(A_BOLD);

    mvaddstr(m_y, m_x,
             std::string(m_ants < 10 ? 1 : 0, '0')
                 .append(std::to_string(m_ants))
                 .c_str());

    attrclr();

    Screen::drawBox(m_x - 2, m_y - 1, 5, 3);
}

void AntNest::update() {
    m_spawnTimer %= SPAWN_PERIOD;
    if (m_spawnTimer++ == 0 && m_ants < 99 && player() != nullptr)
        m_ants++;
}

void AntNest::onAdd(Game* game) {
    PlayerUnit::onAdd(game);

    m_game->m_nestMap[m_nestId] = this;
}

void AntNest::collideWith(GameObject& object) {
    auto ant = dynamic_cast<Ant*>(&object);

    if (!ant)
        return;

    if (ant->player() != player()) {
        if (m_ants > 0)
            m_ants--;
        else {
            m_ants = 1;
            changeOwningPlayer(ant->player());
            disableLines();
            m_game->checkWin();
        }
    }
    else if (ant->m_tx == m_x && ant->m_ty == m_y && m_ants < 99)
        m_ants++;
}

uint8_t AntNest::updatePriority() const {
    return 192;
}

bool AntNest::serialize(std::ostream& stream) const {
    return serializeState(stream << "AntNest ");
}

bool AntNest::serializeState(std::ostream& stream) const {
    stream << m_nestId
           << ' ' << m_starting
           << ' ' << static_cast<unsigned short>(m_ants)
           << ' ' << static_cast<unsigned short>(m_spawnTimer)
           << ' ';

    return PlayerUnit::serializeState(stream);
}

bool AntNest::unserialize(std::istream& stream) {
    stream >> m_nestId >> m_starting;

    unsigned short temp;

    stream >> temp;
    m_ants = temp;

    stream >> temp;
    m_spawnTimer = temp;

    return PlayerUnit::unserialize(stream);
}
