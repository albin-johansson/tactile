#pragma once

#include <imgui.h>

#include "core/components/tileset.hpp"
#include "core/region.hpp"

namespace tactile {

struct Viewport;
struct MapInfo;

/// \addtogroup rendering
/// \{

/**
 * \brief Provides useful information about a rendering context.
 *
 * \see get_render_info()
 */
struct render_info final
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

[[nodiscard]] auto get_render_info(const Viewport& viewport, const MapInfo& map)
    -> render_info;

[[nodiscard]] auto get_render_info(const Viewport& viewport, const comp::tileset& tileset)
    -> render_info;

/// \} End of group rendering

}  // namespace tactile
