#pragma once

struct ImVec2;

#include "aliases/col.hpp"
#include "aliases/row.hpp"
#include "core/map/map_position.hpp"

namespace Tactile {

struct CanvasInfo;

struct RenderBounds final
{
  MapPosition begin;
  MapPosition end;
};

[[nodiscard]] auto GetRenderBounds(const CanvasInfo& canvas,
                                   const ImVec2& originPos,
                                   const ImVec2& tileSize,
                                   row_t nRows,
                                   col_t nCols) -> RenderBounds;

}  // namespace Tactile
