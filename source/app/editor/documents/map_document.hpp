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

#include "core/common/ints.hpp"
#include "core/common/math.hpp"
#include "core/fwd.hpp"
#include "core/tools/tool_type.hpp"
#include "editor/document.hpp"

namespace tactile {

/**
 * \brief Represents a loaded map.
 *
 * \details Map document registries have the following context components:
 * - `CommandStack`
 * - `comp::MapInfo`
 * - `ToolManager`
 * - `comp::AttributeContext`
 * - `comp::ActiveAttributeContext`
 * - `comp::ActiveLayer`
 * - `comp::ActiveTileset`
 * - `comp::ActiveObject`
 * - `comp::TilesetContext`
 * - `comp::Viewport`
 */
class MapDocument final : public ADocument
{
 public:
  MapDocument(const Vector2i& tileSize, usize rows, usize columns);

  void update() override;

  [[nodiscard]] auto is_tool_active(ToolType tool) const -> bool;
  [[nodiscard]] auto is_tool_possible(ToolType tool) const -> bool;

  [[nodiscard]] auto info() const -> const comp::MapInfo&;

  [[nodiscard]] auto viewport() const -> const comp::Viewport&;

  [[nodiscard]] auto tile_size() const -> Vector2i;

  [[nodiscard]] auto get_tools() -> ToolManager&;
  [[nodiscard]] auto get_tools() const -> const ToolManager&;

  [[nodiscard]] auto get_type() const -> DocumentType override
  {
    return DocumentType::Map;
  }
};

}  // namespace tactile
