
#include <iostream>

#include "GameObject/Ant.t.cpp"
#include "log.h"

/**
 * @brief testing entry point
 *
 * @return exit code
 */
int main() {
    PN_DEBUG_ONLY(g_PNLogStream = &std::cout);

    testAnt();

    PN_LOG("");
    PN_LOG("looks alright..");
    return 0;
}
