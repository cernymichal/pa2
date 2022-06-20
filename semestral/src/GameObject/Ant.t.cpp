#include "Ant.h"

#include <cassert>

#include "../log.h"

void testAnt() {
    PN_LOGH1("testing ant");

    PN_LOGH2("#1");

    Ant a({10, 23}, nullptr);

    assert(a.location() == Vector2<uint8_t>(10, 23));
}
