#include "Save.h"

#include <algorithm>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "log.h"

#define SAVE_EXTENSION ".pns"
#define MAP_EXTENSION ".pnm"
#define SAVE_DIRECTORY "examples"
#define MAP_DIRECTORY "assets/maps"

const auto EXECUTABLE_DIRECTORY = std::filesystem::canonical("/proc/self/exe").parent_path();

std::vector<Save> findFiles(const char* directory, const char* extension) {
    std::vector<Save> files;

    auto path = std::filesystem::path(EXECUTABLE_DIRECTORY)
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
    auto path = std::filesystem::path(EXECUTABLE_DIRECTORY)
                    .append(directory);

    PN_LOG("checking if any " << extension << " files exist in " << path);

    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (entry.is_regular_file() && entry.path().extension() == extension)
            return true;
    }

    return false;
}

Save::Save(const std::filesystem::path& path) : path(path) {
    std::ifstream saveFile;
    saveFile.open(path, std::fstream::in);

    std::getline(saveFile, _name);
}

bool Save::operator<(const Save& other) const {
    return name() < other.name();
}

const std::string& Save::name() const {
    return _name;
}

const char* Save::c_str() const {
    return _name.c_str();
}

std::vector<Save> Save::findSaves() {
    PN_LOG("looking for saves");
    return findFiles(SAVE_DIRECTORY, SAVE_EXTENSION);
}

std::vector<Save> Save::findMaps() {
    PN_LOG("looking for maps");
    return findFiles(MAP_DIRECTORY, MAP_EXTENSION);
}

bool Save::emptySaveDirectory() {
    try {
        return !filesExist(SAVE_DIRECTORY, SAVE_EXTENSION);
    } catch (std::filesystem::filesystem_error& _) {
        return true;
    }
}

bool Save::emptyMapDirectory() {
    try {
        return !filesExist(MAP_DIRECTORY, MAP_EXTENSION);
    } catch (std::filesystem::filesystem_error& _) {
        return true;
    }
}

std::filesystem::path Save::createSavePath(const std::string& mapName) {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << " " << mapName << SAVE_EXTENSION;

    return std::filesystem::path(EXECUTABLE_DIRECTORY)
        .append(SAVE_DIRECTORY)
        .append(oss.str());
}
