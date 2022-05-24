#include "AntLine.h"

#include "../Game.h"
#include "../Screen/Screen.h"
#include "Ant.h"

AntLine::AntLine() {
}

AntLine::AntLine(char nestAId, char nestBId) : nestAId(nestAId), nestBId(nestBId) {
}

void AntLine::draw() const {
    // TODO colors

    init_pair(1, COLOR_BLACK, color);

    Screen::drawDottedLine(nestA->x, nestA->y, nestB->x, nestB->y);
}

void AntLine::update() {
    if (!nestA)
        nestA = _game->nestMap[nestAId];

    if (!nestB)
        nestB = _game->nestMap[nestBId];

    if (nestAActive && nestA->ants > 0) {
        nestA->ants--;
        _game->addObject(
            new Ant(nestA->x, nestA->y, nestA->player(), nestB->x, nestB->y));
    }

    if (nestBActive && nestB->ants > 0) {
        nestB->ants--;
        _game->addObject(
            new Ant(nestB->x, nestB->y, nestB->player(), nestA->x, nestA->y));
    }
}

bool AntLine::serialize(std::ostream& stream) const {
    return _serialize(stream << "AntLine ");
}

bool AntLine::_serialize(std::ostream& stream) const {
    stream << nestAId << ' ' << nestAActive << ' ' << nestBId << ' ' << nestBActive << ' ';
    return GameObject::_serialize(stream);
}

bool AntLine::unserialize(std::istream& stream) {
    stream >> nestAId >> nestAActive >> nestBId >> nestBActive;
    return GameObject::unserialize(stream);
}
