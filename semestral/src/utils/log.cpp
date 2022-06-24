#include "log.h"

#ifdef PN_DEBUG

#include <fstream>

std::ofstream g_PNLogFile = std::ofstream();
std::ostream* g_PNLogStream = &g_PNLogFile;

#endif
