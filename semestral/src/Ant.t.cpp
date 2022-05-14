#include "Ant.h"

#include <cassert>

#include "log.h"

void testAnt() {
    PN_LOGH("testing ant");

    Ant a(10, 23);

    assert(a.x == 10);
    assert(a.y == 23);
}
