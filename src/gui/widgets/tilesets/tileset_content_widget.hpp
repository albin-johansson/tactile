#pragma once

#include <entt.hpp>  // dispatcher

namespace tactile {

class MapDocument;

void TilesetContentWidget(const MapDocument& document,
                          entt::dispatcher& dispatcher);

}  // namespace tactile
