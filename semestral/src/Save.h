#pragma once

#include <filesystem>
#include <vector>

/**
 * @brief save file path wrapper
 */
class Save {
public:
    std::filesystem::path path;

    /**
     * @brief load save name from save file
     *
     * @param[in] path
     */
    Save(const std::filesystem::path& path);

    /**
     * @param[in] other
     *
     * @returns this save name < other save name
     */
    bool operator<(const Save& other) const;

    /**
     * @returns save name
     */
    const std::string& name() const;

    /**
     * for dialog
     *
     * @returns char* to save name
     */
    const char* c_str() const;

    /**
     * @brief find all saves in default directory
     *
     * @returns vector of Saves
     */
    static std::vector<Save> findSaves();

    /**
     * @brief find all maps in default directory
     *
     * @returns vector of Saves
     */
    static std::vector<Save> findMaps();

    /**
     * @brief create save file path from map name
     *
     * "%Y-%m-%d %H:%M:%S %mapName"
     *
     * @returns path
     */
    static std::filesystem::path createSavePath(const std::string& mapName);

private:
    std::string _name;
};
