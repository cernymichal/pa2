#include "AntNest.h"

#include "../Game.h"
#include "../Screen/Screen.h"
#include "Ant.h"

AntNest::AntNest() : AntNest(0, 0, false) {
}

AntNest::AntNest(uint8_t x, uint8_t y, char id, bool starting) : GameObject(x, y), id(id), starting(starting) {
    // make sure ants are being spawned after ant movement
    updatePriority = 192;
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

    if (ants > 0) {
        _game->addObject(
            new Ant(x, y));
        ants--;
    }
}

void AntNest::collideWith(GameObject& object) {
    auto objectP = &object;

    if (object.color != color && dynamic_cast<Ant*>(objectP)) {
        ants--;
        // TODO capture
    }
}

bool AntNest::serialize(std::ostream& stream) const {
    return _serialize(stream << "AntNest ");
}

bool AntNest::_serialize(std::ostream& stream) const {
    stream << id << ' ' << starting << ' ' << (unsigned short)ants << ' ';
    return GameObject::_serialize(stream);
}

bool AntNest::unserialize(std::istream& stream) {
    stream >> id >> starting;

    unsigned short temp;
    stream >> temp;
    ants = temp;

    return GameObject::unserialize(stream);
}
