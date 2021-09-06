#pragma once

#include <ImGuiFileDialog.h>

#include <filesystem>  // path
#include <string>      // string

#include "aliases/cstr.hpp"

namespace Tactile {

/// \addtogroup gui
/// \{

/// \name File dialog API
/// \{

/**
 * \brief Represents different file dialog states.
 *
 * \see `FileDialog()`
 * \see `FileDialogExport()`
 * \see `FileDialogImport()`
 * \see `GetFileDialogSelectedPath()`
 */
enum class FileDialogResult
{
  Idle,     ///< The file dialog is open.
  Success,  ///< A file was selected.
  Close     ///< The file dialog was closed.
};

/**
 * \brief Shows a file dialog.
 *
 * \param id the unique ID associated with the dialog.
 * \param title the title of the window.
 * \param filter the file type filter, e.g. ".foo,.bar".
 * \param flags the flags that will be used.
 *
 * \return `Idle` if the dialog is shown but no file has been chosen;
 *         `Success` if the a file was selected;
 *         `Close` if the dialog was closed.
 *
 * \see `FileDialogExport()`
 * \see `FileDialogImport()`
 */
auto FileDialog(const std::string& id,
                const std::string& title,
                CStr filter,
                ImGuiFileDialogFlags flags) -> FileDialogResult;

/**
 * \brief Shows a file dialog for exporting files.
 *
 * \param id the unique ID associated with the file dialog.
 * \param title the title of the window.
 * \param filter the file type filter, e.g. ".foo,.bar".
 *
 * \return `Idle` if the dialog is shown but no file has been chosen;
 *         `Success` if the a file was selected;
 *         `Close` if the dialog was closed.
 *
 * \see `FileDialogImport()`
 */
auto FileDialogExport(const std::string& id, const std::string& title, CStr filter)
    -> FileDialogResult;

/**
 * \brief Shows a file dialog for importing files.
 *
 * \param id the unique ID associated with the file dialog.
 * \param title the title of the window.
 * \param filter the file type filter, e.g. ".foo,.bar".
 *
 * \return `Idle` if the dialog is shown but no file has been chosen;
 *         `Success` if the a file was selected;
 *         `Close` if the dialog was closed.
 *
 * \see `FileDialogExport()`
 */
auto FileDialogImport(const std::string& id, const std::string& title, CStr filter)
    -> FileDialogResult;

/// Returns the file path to the last selected file.
[[nodiscard]] auto GetFileDialogSelectedPath() -> std::filesystem::path;

/// \} End of file dialog API

/// \} End of group gui

}  // namespace Tactile
