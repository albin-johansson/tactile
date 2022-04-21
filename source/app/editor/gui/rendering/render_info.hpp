/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <imgui.h>

#include "core/fwd.hpp"
#include "core/region.hpp"

namespace tactile {

struct MapInfo;

/// \addtogroup rendering
/// \{

/**
 * \brief Provides useful information about a rendering context.
 *
 * \see get_render_info()
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

[[nodiscard]] auto get_render_info(const comp::Viewport& viewport, const MapInfo& map)
    -> RenderInfo;

[[nodiscard]] auto get_render_info(const comp::Viewport& viewport,
                                   const comp::Tileset& tileset) -> RenderInfo;

/// \} End of group rendering

}  // namespace tactile
