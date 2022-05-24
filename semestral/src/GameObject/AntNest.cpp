#include "AntNest.h"

#include "../Game.h"
#include "../Screen/Screen.h"
#include "Ant.h"

AntNest::AntNest() : AntNest(0, 0, false) {
}

AntNest::AntNest(uint8_t x, uint8_t y, char id, bool starting) : PlayerUnit(x, y), id(id), starting(starting) {
    // make sure ants are being spawned after ant movement
    updatePriority = 192;
    color = COLOR_WHITE;
}

void AntNest::draw() const {
    // TODO colors
    // TODO bold id
    init_pair(1, COLOR_BLACK, color);

    mvaddch(y, x - 1, id);
    mvaddstr(y, x,
             std::string(ants < 10 ? 1 : 0, '0')
                 .append(std::to_string(ants))
                 .c_str());

    Screen::drawBox(x - 2, y - 1, 5, 3);
}

void AntNest::update() {
    if (ants < 99)
        ants++;
}

void AntNest::afterAdd() {
    _game->nestMap[id] = this;
}

void AntNest::collideWith(GameObject& object) {
    auto ant = dynamic_cast<Ant*>(&object);

    if (ant && ant->player() != player()) {
        if (ants > 0)
            ants--;
        else
            changePlayer(ant->player());
    }
}

bool AntNest::serialize(std::ostream& stream) const {
    return _serialize(stream << "AntNest ");
}

bool AntNest::_serialize(std::ostream& stream) const {
    stream << id << ' ' << starting << ' ' << (unsigned short)ants << ' ';
    return PlayerUnit::_serialize(stream);
}

bool AntNest::unserialize(std::istream& stream) {
    stream >> id >> starting;

    unsigned short temp;
    stream >> temp;
    ants = temp;

    return PlayerUnit::unserialize(stream);
}
