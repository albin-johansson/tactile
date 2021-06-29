#pragma once

#include <entt.hpp>  // dispatcher

namespace Tactile {

class MapDocument;

void TilesetContentWidget(const MapDocument& document,
                          entt::dispatcher& dispatcher);

}  // namespace Tactile
