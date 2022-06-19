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

        PN_LOG("deleting " << m_options[m_optionIndex].path());

        try {
            std::filesystem::remove(m_options[m_optionIndex].path());
        }
        catch (std::filesystem::filesystem_error&) {
        }

        m_options.erase(m_options.begin() + m_optionIndex);

        if (m_optionIndex >= m_options.size())
            m_optionIndex--;

        if (m_options.empty()) {
            m_closed = true;
            m_exit = true;
        }
    }

    Dialog<Save>::update(dt, key);
}
