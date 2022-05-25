#include "AntLine.h"

#include <ncurses.h>

#include "../Game.h"
#include "../Screen/Screen.h"
#include "Ant.h"

AntLine::AntLine() {
    // draw line and spaw ants before movement
    updatePriority = 64;
}

AntLine::AntLine(char nestAId, char nestBId) : nestAId(nestAId), nestBId(nestBId) {
    // draw line and spaw ants before movement
    updatePriority = 64;
}

void AntLine::switchSide(AntNest* nest, bool value) {
    if (nestA == nest)
        nestAActive = value;
    else
        nestBActive = value;
}

AntNest* AntLine::otherNest(AntNest* nest) {
    if (nestA == nest)
        return nestB;
    else
        return nestA;
}

bool AntLine::otherSideActive(AntNest* nest) {
    if (nestA == nest)
        return nestBActive;
    else
        return nestAActive;
}

bool AntLine::friendly() {
    return nestA->player() == nestB->player();
}

void AntLine::draw() const {
    attron(COLOR_PAIR(color));
    Screen::drawDottedLine(nestA->x, nestA->y, nestB->x, nestB->y);
}

void AntLine::update() {
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

void AntLine::onLoad() {
    nestA = _game->nestMap[nestAId];
    nestB = _game->nestMap[nestBId];

    nestA->lineMap[nestBId] = this;
    nestB->lineMap[nestAId] = this;
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
