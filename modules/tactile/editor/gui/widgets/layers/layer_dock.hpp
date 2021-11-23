#pragma once

#include <tactile_def.hpp>

#include <entt/entt.hpp>  // registry, dispatcher

namespace Tactile {

class Icons;

/**
 * \brief Updates the state of the layer dock widget.
 *
 * \ingroup gui
 *
 * \param registry the currently active registry.
 * \param icons the loaded icons.
 * \param dispatcher the event dispatcher that will be used.
 */
void UpdateLayerDock(const entt::registry& registry,
                     const Icons& icons,
                     entt::dispatcher& dispatcher);

void OpenRenameLayerDialog(LayerID id);

[[nodiscard]] auto IsLayerDockFocused() noexcept -> bool;

}  // namespace Tactile
