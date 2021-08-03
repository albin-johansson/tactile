#pragma once

#include <centurion.hpp>  // frect

struct ImVec2;

#include "aliases/col.hpp"
#include "aliases/row.hpp"
#include "core/map/map_position.hpp"

namespace Tactile {

struct CanvasInfo;
struct RenderInfo;

struct RenderBounds final
{
  MapPosition begin;
  MapPosition end;
};

[[nodiscard]] auto GetRenderBounds(const CanvasInfo& canvas, const RenderInfo& info)
    -> RenderBounds;

[[nodiscard]] auto ConvertBoundsToRect(const RenderInfo& info) -> cen::frect;

}  // namespace Tactile
