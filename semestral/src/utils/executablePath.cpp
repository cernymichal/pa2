#include "executablePath.h"

// https://stackoverflow.com/a/60250581

#ifdef _WIN32

#include <windows.h>

std::string getExecutablePath() {
    char rawPath[MAX_PATH];
    GetModuleFileNameA(NULL, rawPath, sizeof(rawPath));
    return std::filesystem::canonical(rawPath);
}

#endif

#ifdef __linux__

#ifdef __sun
#define PROC_SELF_EXE "/proc/self/path/a.out"
#else
#define PROC_SELF_EXE "/proc/self/exe"
#endif

std::filesystem::path getExecutablePath() {
    return std::filesystem::canonical(PROC_SELF_EXE);
}

#endif

#ifdef __APPLE__

#include <limits.h>
#include <mach-o/dyld.h>

std::string getExecutablePath() {
    char rawPath[PATH_MAX];
    uint32_t rawPathSize = (uint32_t)sizeof(rawPath);

    _NSGetExecutablePath(rawPath, &rawPathSize);

    return std::filesystem::canonical(rawPath);
}

#endif
