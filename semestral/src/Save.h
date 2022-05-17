#pragma once

#include <filesystem>
#include <vector>

// TODO docs
class Save {
public:
    std::filesystem::path path;

    Save(const std::filesystem::path& path);

    bool operator<(const Save& other) const;

    const std::string& name() const;

    const char* c_str() const;

    static std::vector<Save> findSaves();

    static std::vector<Save> findMaps();

    static std::filesystem::path createSavePath(const std::string& mapName);

private:
    std::string _name;
};
