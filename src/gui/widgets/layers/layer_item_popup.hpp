#pragma once

#include <entt.hpp>  // dispatcher

#include "aliases/layer_id.hpp"

namespace Tactile {

class MapDocument;

void UpdateLayerItemPopup(const MapDocument& document,
                          entt::dispatcher& dispatcher,
                          layer_id id);

}  // namespace Tactile
