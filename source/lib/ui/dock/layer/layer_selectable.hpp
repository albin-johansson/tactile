// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <entt/fwd.hpp>

namespace tactile {
class MapDocument;
class Layer;
}  // namespace tactile

namespace tactile::ui {

/// Shows a widget for a single layer in a map.
void layer_selectable(const MapDocument& document,
                      const Layer& layer,
                      entt::dispatcher& dispatcher);

}  // namespace tactile::ui
