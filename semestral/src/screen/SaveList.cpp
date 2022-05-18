#include "SaveList.h"

#include <ncurses.h>

#include <filesystem>

#include "../log.h"

SaveList::SaveList(Application& application, std::function<void(Dialog<Save>&, Application&)> onExit)
    : Dialog<Save>(application, "save list screen", Save::findSaves(), onExit) {
}

void SaveList::update(std::chrono::nanoseconds dt, int key) {
    // TODO check for fs errors

    if (key == KEY_DC) {
        key = ERR;

        PN_LOG("deleting " << options[optionIndex].path);

        std::filesystem::remove(options[optionIndex].path);
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
