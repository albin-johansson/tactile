#pragma once

#include <tactile_io.hpp>

namespace Tactile {

/// \addtogroup gui
/// \{

/**
 * \brief Updates the map parsing error dialog.
 *
 * \see `OpenMapImportErrorDialog()`
 */
void UpdateMapImportErrorDialog();

/**
 * \brief Opens the map parsing error dialog.
 *
 * \param error the parse error that will be displayed.
 */
void OpenMapImportErrorDialog(IO::ParseError error);

/// \} End of group gui

}  // namespace Tactile
