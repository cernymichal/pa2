#include "SaveList.h"

#include <ncurses.h>

#include <filesystem>

#include "../log.h"

SaveList::SaveList(Application& application, const std::vector<Save>& options, std::function<void(Dialog<Save>&, Application&)> onExit)
    : Dialog<Save>(application, "save list screen", options, onExit) {
}

void SaveList::update(std::chrono::nanoseconds dt, int key) {
    if (key == KEY_DC) {
        key = ERR;

        PN_LOG("deleting " << options[optionIndex].path);

        try {
            std::filesystem::remove(options[optionIndex].path);
        }
        catch (std::filesystem::filesystem_error& _) {
        }

        options.erase(options.begin() + optionIndex);

        if (optionIndex >= options.size())
            optionIndex--;

        if (options.empty()) {
            closed = true;
            exit = true;
        }
    }

    Dialog<Save>::update(dt, key);
}
