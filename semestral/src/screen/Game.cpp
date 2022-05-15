#include "Game.h"

#include <ncurses.h>

#include <algorithm>
#include <iostream>

#include "../log.h"

const auto FRAME_TIME = std::chrono::milliseconds(500);

Game::Game() {
    timeoutDelay = 0;  // non blocking input
    clear();
}

void Game::resetScreen() {
    exit = false;
    paused = false;
    _dtAccumulator = FRAME_TIME;
}

void Game::clear() {
    resetScreen();
    _objects.clear();
}

void Game::addObject(GameObject *object) {
    auto compareGameObjects = [](const GameObject *a, const std::unique_ptr<GameObject> &b) {
        return a->updatePriority < b->updatePriority;
    };

    _objects.emplace(
        std::upper_bound(_objects.begin(), _objects.end(), object, compareGameObjects),
        static_cast<GameObject *>(object));
}

void Game::update(std::chrono::nanoseconds dt, int key) {
    _dtAccumulator += dt;

    // PN_LOG("Game::update(" << (long long)dt.count() << ", " << (char)key << ")");

    switch (key) {
        case ERR:
            break;

        case KEY_BACKSPACE:
            paused = true;
        default:
            exit = true;
            break;
    }

    if (_dtAccumulator > FRAME_TIME) {
        _dtAccumulator -= FRAME_TIME;

        for (std::unique_ptr<GameObject> &object : _objects)
            object->update();

        _draw();
    }
}

void Game::_draw() {
    ::clear();

    for (std::unique_ptr<GameObject> &object : _objects)
        object->draw();

    refresh();
}

bool Game::save(const char *fileName) const {
    // TODO save game

    return false;
}

Game Game::load(const char *fileName) {
    // TODO load game

    return Game();
}

std::ostream &Game::log(std::ostream &stream) const {
    stream << "objects: {" << std::endl;

    for (const std::unique_ptr<GameObject> &object : _objects)
        object->log(stream) << "," << std::endl;

    return stream << "}";
}
