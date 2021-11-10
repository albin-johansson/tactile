#pragma once

#include <entt/entt.hpp>  // dispatcher

namespace Tactile {

class Model;

/// \addtogroup gui
/// \{

/**
 * \brief Updates the "Edit" menu.
 *
 * \param model the associated model.
 * \param dispatcher the event dispatcher that will be used.
 *
 * \see `UpdateEditMenuWindows()`
 */
void UpdateEditMenu(const Model& model, entt::dispatcher& dispatcher);

/**
 * \brief Updates the windows associated with the "Edit" menu.
 *
 * \param dispatcher the event dispatcher that will be used.
 */
void UpdateEditMenuWindows(entt::dispatcher& dispatcher);

void ShowSettingsDialog() noexcept;

/// \} End of group gui

}  // namespace Tactile
