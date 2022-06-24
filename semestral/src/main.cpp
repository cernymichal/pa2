#include "Application.h"
#include "utils/log.h"

/**
 * @brief entry point
 *
 * log init is called here
 *
 * @param[in] argc argument count
 * @param[in] argv argument array
 *
 * @return exit code
 */
int main(int argc, char** argv) {
    PN_INIT_LOG();

    Application().start();

    PN_LOGH2("main end");
    return 0;
}
