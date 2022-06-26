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
#include <vector>  // vector

#include "core/common/ints.hpp"
#include "core/common/math.hpp"
#include "core/common/memory.hpp"
#include "core/common/tile_cache.hpp"
#include "core/common/uuid.hpp"
#include "core/documents/document.hpp"
#include "core/fwd.hpp"
#include "core/layer_type.hpp"
#include "core/map.hpp"
#include "core/tools/tool_manager.hpp"
#include "core/tools/tool_type.hpp"
#include "core/viewport.hpp"

namespace tactile {

/// Represents a loaded map.
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

  void register_stamp_sequence(const UUID& layerId,
                               TileCache   previous,
                               TileCache   sequence);

  void register_eraser_sequence(const UUID& layerId, TileCache previous);

  void flood(const UUID& layerId, const TilePos& origin, TileID replacement);

  void add_rectangle(const UUID& layerId, const Vector2f& pos, const Vector2f& size);

  void add_ellipse(const UUID& layerId, const Vector2f& pos, const Vector2f& size);

  void add_point(const UUID& layerId, const Vector2f& pos);

  void move_object(const UUID&     layerId,
                   const UUID&     objectId,
                   const Vector2f& previous,
                   const Vector2f& updated);

  void set_object_visible(const UUID& layerId, const UUID& objectId, bool visible);

  void set_object_tag(const UUID& layerId, const UUID& objectId, std::string tag);

  void set_name(std::string name) override;

  [[nodiscard]] auto get_name() const -> const std::string& override;

  [[nodiscard]] auto get_tools() -> ToolManager&;
  [[nodiscard]] auto get_tools() const -> const ToolManager&;

  [[nodiscard]] auto get_viewport() -> core::Viewport& override { return mViewport; }

  [[nodiscard]] auto get_viewport() const -> const core::Viewport& override
  {
    return mViewport;
  }

  [[nodiscard]] auto get_type() const -> DocumentType override
  {
    return DocumentType::Map;
  }

  [[nodiscard]] auto get_map() -> core::Map& { return mMap; }
  [[nodiscard]] auto get_map() const -> const core::Map& { return mMap; }

 private:
  core::Map      mMap;
  core::Viewport mViewport;
  ToolManager    mTools;
  // TODO active context
};

}  // namespace tactile
