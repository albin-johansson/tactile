#pragma once

#include <entt.hpp>  // dispatcher

namespace Tactile {

class Tileset;

void TilesetViewportWidget(const Tileset& tileset,
                           entt::dispatcher& dispatcher);

}  // namespace Tactile
