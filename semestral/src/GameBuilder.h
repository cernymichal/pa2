#pragma once

#include <filesystem>

#include "Game.h"
#include "utils/log.h"

class GameBuilder {
public:
    GameBuilder();

    std::unique_ptr<Game> getGame();

    /**
     * @brief create players in game
     *
     * - 1 * Player + aiPlayers * ComputerPlayers
     * - randomly assing starting points
     *
     * @param[in] aiPlayers count of ComputerPlayers
     */
    void createPlayers(uint8_t aiPlayers = 0);

    /**
     * @brief count starting nests
     *
     * @return max number of players
     */
    uint8_t maxPlayers() const;

    /**
     * @brief load Game from file
     *
     * - throws SaveException on fail
     *
     * @param[in] path save file
     */
    void loadFromFile(const std::filesystem::path& path);

    /**
     * @brief save Game to file
     *
     * - saves save name, map name and serializes all GameObjects
     * - throws SaveException on fail
     *
     * @param[in] game
     * @param[in] path target
     */
    static void saveToFile(const Game& game, const std::filesystem::path& path);

    /**
     * @brief register GameObject for loading
     */
    template <typename T>
    static void registerGameObject() {
        auto name = T().name();
        PN_LOG("GameBuilder::registerGameObject<" << name << ">()");
        s_GameObjectInstatiators[T().name()] = &instantiateGameObject<T>;
    }

private:
    std::unique_ptr<Game> m_game;

    static std::map<std::string, GameObject* (*)()> s_GameObjectInstatiators;

    template <typename T>
    static GameObject* instantiateGameObject() { return new T; }
};
