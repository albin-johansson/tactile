#pragma once

#include <entt.hpp>  // registry, dispatcher

#include "common/layer_id.hpp"

namespace Tactile {

/**
 * \brief Updates the state of the layer dock widget.
 *
 * \ingroup gui
 *
 * \param registry the currently active registry.
 * \param dispatcher the event dispatcher that will be used.
 */
void UpdateLayerDock(const entt::registry& registry, entt::dispatcher& dispatcher);

void OpenRenameLayerDialog(LayerID id);

[[nodiscard]] auto IsLayerDockFocused() noexcept -> bool;

}  // namespace Tactile
