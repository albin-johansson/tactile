#pragma once

#include <entt/entt.hpp>  // dispatcher

namespace Tactile {

/// \addtogroup gui
/// \{

/**
 * \brief Updates the layer creation popup.
 *
 * \param dispatcher the event dispatcher that will be used.
 *
 * \see `OpenAddLayerPopup()`
 */
void UpdateAddLayerPopup(entt::dispatcher& dispatcher);

/**
 * \brief Opens the layer creation popup.
 *
 * \see `UpdateAddLayerPopup()`
 */
void OpenAddLayerPopup();

/// \} End of group gui

}  // namespace Tactile
