#pragma once

#include <filesystem>  // path
#include <string>      // string

#include "aliases/czstring.hpp"

namespace tactile {

enum class FileDialogResult
{
  Idle,
  Success,
  Close
};

/**
 * \brief Shows a file dialog.
 *
 * \param id the unique ID associated with the dialog.
 * \param title the title of the window.
 * \param filter the file type filter, e.g. ".foo,.bar".
 *
 * \return `Idle` if the dialog is shown but no file has been chosen; `Success`
 * if the a file was selected; `Close` if the dialog was closed.
 */
auto ShowFileDialog(const std::string& id,
                    const std::string& title,
                    czstring filter) -> FileDialogResult;

/// Returns the file path to the last selected file.
[[nodiscard]] auto GetFileDialogSelectedPath() -> std::filesystem::path;

}  // namespace tactile
