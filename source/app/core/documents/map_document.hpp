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

#include <string>  // string

#include "core/common/ints.hpp"
#include "core/common/math.hpp"
#include "core/common/uuid.hpp"
#include "core/documents/document.hpp"
#include "core/fwd.hpp"
#include "core/layer_type.hpp"
#include "core/tools/tool_type.hpp"

namespace tactile {

/**
 * \brief Represents a loaded map.
 *
 * \details Map document registries have the following context components:
 * - `CommandStack`
 * - `comp::MapInfo`
 * - `ToolManager`
 * - `comp::Context`
 * - `comp::ActiveContext`
 * - `comp::ActiveLayer`
 * - `comp::ActiveTileset`
 * - `comp::ActiveObject`
 * - `comp::ContextMapping`
 * - `comp::TilesetContext`
 * - `comp::Viewport`
 */
class MapDocument final : public ADocument
{
 public:
  MapDocument(const Vector2i& tileSize, usize rows, usize columns);

  void update() override;

  void add_row();

  void add_column();

  void remove_row();

  void remove_column();

  void resize(usize rows, usize cols);

  void fix_tiles();

  void add_layer(LayerType type);

  void remove_layer(const UUID& layerId);

  void duplicate_layer(const UUID& layerId);

  void rename_layer(const UUID& layerId, std::string name);

  void move_layer_up(const UUID& layerId);

  void move_layer_down(const UUID& layerId);

  void set_layer_opacity(const UUID& layerId, float opacity);

  void set_layer_visible(const UUID& layerId, bool visible);

  void move_object(const UUID& objectId,
                   const Vector2f& previous,
                   const Vector2f& updated);

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
