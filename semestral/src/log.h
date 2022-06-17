#pragma once

#ifdef PN_DEBUG

#include <fstream>

#define PN_DEBUG_ONLY(x) x

extern std::ofstream g_PNLogFile;
extern std::ostream* g_PNLogStream;

#else

/**
 * @brief run only in debug mode
 *
 * @param[in] x expression to run
 */
#define PN_DEBUG_ONLY(x)

#endif

/**
 * @brief log message
 *
 * @param[in] x printable value
 */
#define PN_LOG(x) PN_DEBUG_ONLY((*g_PNLogStream) << x << std::endl)

/**
 * @brief log message as a large header
 *
 * @param[in] x printable value
 */
#define PN_LOGH1(x) PN_DEBUG_ONLY((*g_PNLogStream) << "\n\n" \
                                                  << "==================== " << x << " ====================" << std::endl)

/**
 * @brief log message as a header
 *
 * @param[in] x printable value
 */
#define PN_LOGH2(x) PN_DEBUG_ONLY((*g_PNLogStream) << "\n" \
                                                  << "=> " << x << std::endl)

/**
 * @brief log object
 *
 * @param[in] x pointer to an object with log method
 */
#define PN_LOG_OBJ(x) PN_DEBUG_ONLY((x)->log(*g_PNLogStream) << std::endl)

/**
 * @brief open log file
 */
#define PN_INIT_LOG() PN_DEBUG_ONLY(g_PNLogFile.open("log", std::fstream::out | std::fstream::app))
