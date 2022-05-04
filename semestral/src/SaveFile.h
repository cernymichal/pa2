#pragma once

#include "Game.h"

bool saveGame(const Game &game, const char *fileName);
Game loadGame(const char *fileName);
