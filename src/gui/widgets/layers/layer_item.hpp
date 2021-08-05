#pragma once

#include <entt.hpp>  // dispatcher

#include "aliases/layer_id.hpp"
#include "core/map/layers/layer.hpp"

namespace Tactile {

class MapDocument;

void LayerItem(const MapDocument& document,
               entt::dispatcher& dispatcher,
               layer_id id,
               const SharedLayer& layer);

}  // namespace Tactile
