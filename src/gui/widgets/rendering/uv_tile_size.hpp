#pragma once

#include <imgui.h>

#include "aliases/tile_id.hpp"

namespace Tactile {

class Tileset;

[[nodiscard]] auto GetTileSizeUV(const Tileset& tileset) -> ImVec2;

}  // namespace Tactile
