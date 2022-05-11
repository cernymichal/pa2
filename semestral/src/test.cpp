#include <cassert>
#include <iostream>

#include "Ant.h"
#include "AntNest.h"
#include "ComputerPlayer.h"
#include "Game.h"
#include "GameObject.h"
#include "Label.h"
#include "Player.h"
#include "Program.h"
#include "Wall.h"

/**
 * @brief testing entry point
 *
 * @param[in] argc argument count
 * @param[in] argv argument array
 *
 * @return exit code
 */
int main(int argc, char **argv) {
    std::cout << "testing arithmetic" << std::endl;
    assert(1 + 1 == 2);

    std::cout << "everything looks fine :)" << std::endl;
    return 0;
}
