#pragma once

#include <entt.hpp>  // dispatcher

namespace tactile {

class Tileset;

void TilesetViewportWidget(const Tileset& tileset,
                           entt::dispatcher& dispatcher);

}  // namespace tactile
