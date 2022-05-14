#include "log.h"

#ifdef PN_DEBUG

#include <fstream>

std::ofstream PN_LOGFILE = std::ofstream();
std::ostream* PN_LOGSTREAM = &PN_LOGFILE;

#endif
