#pragma once

#include <imgui.h>

#include "render_bounds.hpp"

namespace Tactile {

class MapDocument;
struct CanvasInfo;

struct RenderInfo final
{
  RenderBounds bounds;    ///< The area of the map that should be rendered.
  ImVec2 map_position{};  ///< Absolute screen position of the map.
  ImVec2 grid_size{};     ///< Graphical tile size.
  ImVec2 tile_size{};     ///< Logical tile size.
  float row_count{};      ///< Total amount of rows.
  float col_count{};      ///< Total amount of columns.
};

[[nodiscard]] auto GetRenderInfo(const MapDocument& document,
                                 const CanvasInfo& canvas) -> RenderInfo;

}  // namespace Tactile
