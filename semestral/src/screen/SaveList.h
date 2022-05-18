#pragma once

#include "../Save.h"
#include "Dialog.h"

/**
 * @brief dialog with file deletion
 *
 * delete option with DEL, option.path will be deleted from filesystem
 */
class SaveList : public Dialog<Save> {
public:
    /**
     * @param[in] application
     * @param[in] options save files
     * @param[in] onExit function to be called after exit
     */
    SaveList(Application& application, const std::vector<Save>& options, std::function<void(Dialog<Save>&, Application&)> onExit);

    /**
     * @brief check for DEL and call Dialog::update
     *
     * @param[in] dt delta time since last update
     * @param[in] key ncurses input key
     */
    virtual void update(std::chrono::nanoseconds dt, int key) override;
};
