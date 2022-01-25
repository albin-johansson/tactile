#pragma once

#include <imgui.h>

#include "core/region.hpp"

namespace tactile {

struct Viewport;
struct MapInfo;
struct Tileset;

/// \addtogroup rendering
/// \{

/**
 * \brief Provides useful information about a rendering context.
 *
 * \todo Make this work for non-map contexts.
 *
 * \see `GetRenderInfo()`
 */
struct RenderInfo final
{
  ImVec2 canvas_tl{};  ///< Top-left point of the canvas.
  ImVec2 canvas_br{};  ///< Bottom-right point of the canvas.

  Region bounds;  ///< The region that that should be rendered.

  ImVec2 origin{};     ///< Origin screen position.
  ImVec2 grid_size{};  ///< Graphical tile size.
  ImVec2 tile_size{};  ///< Logical tile size.
  ImVec2 ratio{};      ///< Graphical tile size divided by logical tile size.

  float row_count{};  ///< Total amount of rows.
  float col_count{};  ///< Total amount of columns.
};

[[nodiscard]] auto GetRenderInfo(const Viewport& viewport, const MapInfo& map)
    -> RenderInfo;

[[nodiscard]] auto GetRenderInfo(const Viewport& viewport, const Tileset& tileset)
    -> RenderInfo;

/// \} End of group rendering

}  // namespace tactile
