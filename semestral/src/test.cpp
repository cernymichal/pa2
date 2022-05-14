
#include <iostream>

#include "game_object/Ant.t.cpp"
#include "log.h"

/**
 * @brief testing entry point
 *
 * @return exit code
 */
int main() {
    PN_DEBUG_ONLY(PN_LOGSTREAM = &std::cout);

    testAnt();

    PN_LOG("");
    PN_LOG("looks alright..");
    return 0;
}
