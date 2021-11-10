#pragma once

#include <entt/entt.hpp>  // dispatcher

namespace Tactile {

/// \addtogroup gui
/// \{

/**
 * \brief Updates the "Save As" dialog.
 *
 * \param dispatcher the event dispatcher that will be used.
 *
 * \see `OpenSaveAsDialog()`
 */
void UpdateSaveAsDialog(entt::dispatcher& dispatcher);

/**
 * \brief Opens the "Save As" dialog.
 *
 * \see `UpdateSaveAsDialog()`
 */
void OpenSaveAsDialog();

/// \} End of group gui

}  // namespace Tactile
