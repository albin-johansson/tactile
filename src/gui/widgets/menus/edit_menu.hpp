#pragma once

#include <entt.hpp>  // dispatcher

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

/**
 * \brief Opens the dialog for adding new tilesets.
 */
void ShowTilesetDialog() noexcept;

/// \} End of group gui

}  // namespace Tactile
