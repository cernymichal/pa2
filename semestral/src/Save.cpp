#include "Save.h"

#include <algorithm>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "log.h"

const auto g_SaveExtension = ".pns";
const auto g_MapExtension = ".pnm";
const auto g_SaveDirectory = "examples";
const auto g_MapDirectory = "assets/maps";

const auto g_executableDirectory = std::filesystem::canonical("/proc/self/exe").parent_path();

std::vector<Save> findFiles(const char* directory, const char* extension) {
    std::vector<Save> files;

    auto path = std::filesystem::path(g_executableDirectory)
                    .append(directory);

    PN_LOG("looking for " << extension << " files in " << path);

    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (entry.is_regular_file() && entry.path().extension() == extension)
            files.emplace_back(entry.path());
    }

    std::sort(files.begin(), files.end());

    return files;
}

bool filesExist(const char* directory, const char* extension) {
    auto path = std::filesystem::path(g_executableDirectory)
                    .append(directory);

    PN_LOG("checking if any " << extension << " files exist in " << path);

    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (entry.is_regular_file() && entry.path().extension() == extension)
            return true;
    }

    return false;
}

Save::Save(const std::filesystem::path& path) : m_path(path) {
    std::ifstream saveFile;
    saveFile.open(path, std::fstream::in);

    std::getline(saveFile, m_name);
}

bool Save::operator<(const Save& other) const {
    return name() < other.name();
}

const std::string& Save::name() const {
    return m_name;
}

const char* Save::c_str() const {
    return m_name.c_str();
}

std::vector<Save> Save::findSaves() {
    PN_LOG("looking for saves");
    return findFiles(g_SaveDirectory, g_SaveExtension);
}

std::vector<Save> Save::findMaps() {
    PN_LOG("looking for maps");
    return findFiles(g_MapDirectory, g_MapExtension);
}

bool Save::emptySaveDirectory() {
    try {
        return !filesExist(g_SaveDirectory, g_SaveExtension);
    }
    catch (std::filesystem::filesystem_error&) {
        return true;
    }
}

bool Save::emptyMapDirectory() {
    try {
        return !filesExist(g_MapDirectory, g_MapExtension);
    }
    catch (std::filesystem::filesystem_error&) {
        return true;
    }
}

std::filesystem::path Save::createSavePath(const std::string& mapName) {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << " " << mapName << g_SaveExtension;

    return std::filesystem::path(g_executableDirectory)
        .append(g_SaveDirectory)
        .append(oss.str());
}
