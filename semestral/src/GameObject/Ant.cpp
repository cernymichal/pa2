#include "Ant.h"

#include <ncurses.h>

#include "../log.h"
#include "../utils.cpp"
#include "AntNest.h"

Ant::Ant() {
    hitDistance = 0;
}

Ant::Ant(uint8_t x, uint8_t y, Player* player, int8_t tx, int8_t ty) : PlayerUnit(x, y, player), tx(tx), ty(ty) {
    hitDistance = 0;
    mvx = x;
    mvy = y;
}

void Ant::draw() const {
    mvaddch(y, x, 'm' | COLOR_PAIR(color));
}

void Ant::update() {
    if (x != tx || y != ty) {
        float dx = tx - x;
        float dy = ty - y;
        float d = sqrt(dx * dx + dy * dy);
        dx /= d;
        dy /= d;

        mvx += dx;
        mvy += dy;

        x = mvx;
        y = mvy;
    }
}

void Ant::collideWith(GameObject& object) {
    if (dead)
        return;

    auto ant = dynamic_cast<Ant*>(&object);

    if (ant && ant->player() != player()) {
        dead = true;
        return;
    }

    auto nest = dynamic_cast<AntNest*>(&object);

    if (nest && nest->x == tx && nest->y == ty)
        dead = true;
}

bool Ant::serialize(std::ostream& stream) const {
    return _serialize(stream << "Ant ");
}

bool Ant::_serialize(std::ostream& stream) const {
    stream << (short)tx << ' ' << (short)ty << ' ' << mvx << ' ' << mvy << ' ';
    return PlayerUnit::_serialize(stream);
}

bool Ant::unserialize(std::istream& stream) {
    short temp;

    stream >> temp;
    tx = temp;

    stream >> temp;
    ty = temp;

    stream >> mvx >> mvy;

    return PlayerUnit::unserialize(stream);
}
