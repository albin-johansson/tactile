#pragma once

#include <entt.hpp>  // dispatcher

namespace Tactile {

class Model;

/// \addtogroup gui
/// \{

/**
 * \brief Updates the "File" menu.
 *
 * \param model the associated model.
 * \param dispatcher the event dispatcher that will be used.
 *
 * \see `UpdateFileMenuWindows()`
 */
void UpdateFileMenu(const Model& model, entt::dispatcher& dispatcher);

/**
 * \brief Updates the windows associated with the "File" menu.
 *
 * \param dispatcher the event dispatcher that will be used.
 */
void UpdateFileMenuWindows(entt::dispatcher& dispatcher);

/**
 * \brief Opens the map creation dialog.
 */
void ShowAddMapDialog() noexcept;

/**
 * \brief Opens the map import dialog.
 */
void ShowOpenMapDialog() noexcept;

/**
 * \brief Opens the settings dialog.
 */
void ShowSettingsDialog() noexcept;

/// \} End of group gui

}  // namespace Tactile
