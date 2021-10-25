#pragma once

#include <entt.hpp>  // dispatcher

namespace Tactile {

/// \addtogroup gui
/// \{

/**
 * \brief Updates the settings dialog.
 *
 * \param dispatcher the event dispatcher that will be used.
 *
 * \see `OpenSettingsDialog()`
 */
void UpdateSettingsDialog(entt::dispatcher& dispatcher);

/**
 * \brief Opens the settings dialog.
 *
 * \see `UpdateSettingsDialog()`
 */
void OpenSettingsDialog();

/// \} End of group gui

}  // namespace Tactile
