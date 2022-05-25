#include "AntNest.h"

#include <ncurses.h>

#include "../Game.h"
#include "../Screen/Screen.h"
#include "Ant.h"
#include "AntLine.h"

AntNest::AntNest() {
    // make sure ants are being spawned after ant movement
    updatePriority = 192;
    hitDistance = 1;
}

AntNest::AntNest(uint8_t x, uint8_t y, char id, bool starting) : PlayerUnit(x, y), id(id), starting(starting) {
    // make sure ants are being spawned after ant movement
    updatePriority = 192;
    hitDistance = 1;

    if (!starting)
        ants = 10;
}

void AntNest::disableLines() {
    for (auto& line : lineMap)
        line.second->switchSide(this, false);
}

bool AntNest::contested() {
    for (const auto& line : lineMap) {
        if (line.second->otherSideActive(this) && !line.second->friendly())
            return true;
    }

    return false;
}

void AntNest::draw() const {
    attron(COLOR_PAIR(color));
    attron(A_BOLD);

    mvaddch(y, x - 1, id);

    attroff(A_BOLD);

    mvaddstr(y, x,
             std::string(ants < 10 ? 1 : 0, '0')
                 .append(std::to_string(ants))
                 .c_str());

    attrclr();

    Screen::drawBox(x - 2, y - 1, 5, 3);
}

void AntNest::update() {
    _spawnTimer %= 3;
    if (_spawnTimer++ == 0 && ants < 99 && player() != nullptr)
        ants++;
}

void AntNest::onAdd() {
    _game->nestMap[id] = this;
}

void AntNest::collideWith(GameObject& object) {
    auto ant = dynamic_cast<Ant*>(&object);

    if (!ant)
        return;

    if (ant->player() != player()) {
        if (ants > 0)
            ants--;
        else {
            ants = 1;
            changePlayer(ant->player());
            disableLines();
            _game->checkWin();
        }
    }
    else if (ant->tx == x && ant->ty == y && ants < 99)
        ants++;
}

bool AntNest::serialize(std::ostream& stream) const {
    return _serialize(stream << "AntNest ");
}

bool AntNest::_serialize(std::ostream& stream) const {
    stream << id << ' ' << starting << ' ' << (unsigned short)ants << ' ' << (unsigned short)_spawnTimer << ' ';
    return PlayerUnit::_serialize(stream);
}

bool AntNest::unserialize(std::istream& stream) {
    stream >> id >> starting;

    unsigned short temp;

    stream >> temp;
    ants = temp;

    stream >> temp;
    _spawnTimer = temp;

    return PlayerUnit::unserialize(stream);
}
