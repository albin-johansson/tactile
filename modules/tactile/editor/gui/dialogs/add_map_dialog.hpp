#pragma once

#include <entt/entt.hpp>  // dispatcher

namespace Tactile {

/// \addtogroup gui
/// \{

/**
 * \brief Updates the dialog for creating tilemaps.
 *
 * \param dispatcher the event dispatcher that will be used.
 *
 * \see `OpenAddMapDialog()`
 */
void UpdateAddMapDialog(entt::dispatcher& dispatcher);

/**
 * \brief Opens the dialog for creating new tilemaps.
 *
 * \see `UpdateAddMapDialog()`
 */
void OpenAddMapDialog();

/// \} End of group gui

}  // namespace Tactile
