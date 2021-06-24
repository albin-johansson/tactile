#pragma once

#include <filesystem>  // path
#include <string>      // string

#include "aliases/czstring.hpp"

namespace tactile {

/**
 * \brief Shows a file dialog.
 *
 * \param id the unique ID associated with the dialog.
 * \param title the title of the window.
 * \param filter the file type filter, e.g. ".foo,.bar".
 * \param directory the initial directory.
 *
 * \return `true` if the user selected a file; `false` otherwise.
 */
auto ShowFileDialog(const std::string& id,
                    const std::string& title,
                    czstring filter,
                    const std::string& directory) -> bool;

/// Returns the file path to the last selected file.
[[nodiscard]] auto GetFileDialogSelectedPath() -> std::filesystem::path;

}  // namespace tactile
