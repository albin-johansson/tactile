#pragma once

#include "aliases/maybe.hpp"
#include "core/tileset/tileset_selection.hpp"

struct ImVec2;

namespace Tactile {

[[nodiscard]] auto RubberBand(const ImVec2& offset, const ImVec2& tileSize)
    -> Maybe<TilesetSelection>;

}  // namespace Tactile
