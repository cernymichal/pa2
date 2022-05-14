#pragma once

#ifdef PN_DEBUG

#include <fstream>

#define PN_DEBUG_ONLY(x) x

extern std::ofstream PN_LOGFILE;
extern std::ostream* PN_LOGSTREAM;

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
#define PN_LOG(x) PN_DEBUG_ONLY((*PN_LOGSTREAM) << x << std::endl)

/**
 * @brief log message as header
 *
 * @param[in] x printable value
 */
#define PN_LOGH(x) PN_DEBUG_ONLY((*PN_LOGSTREAM) << std::endl \
                                                 << "=> " << x << std::endl)

/**
 * @brief log object
 *
 * @param[in] x pointer to an object with log method
 */
#define PN_LOG_OBJ(x) PN_DEBUG_ONLY((x)->log(*PN_LOGSTREAM) << std::endl)

/**
 * @brief open log file
 */
#define PN_INIT_LOG() PN_DEBUG_ONLY(PN_LOGFILE.open("log", std::fstream::out | std::fstream::app))
