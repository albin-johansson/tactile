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

#include <boost/uuid/uuid_hash.hpp>

#include "core/ctx/context_manager.hpp"
#include "core/document/document.hpp"
#include "core/document/document_delegate.hpp"
#include "core/layer/layer_type.hpp"
#include "core/map.hpp"
#include "core/tool/tool_manager.hpp"
#include "core/tool/tool_type.hpp"
#include "core/type/math.hpp"
#include "core/type/maybe.hpp"
#include "core/type/path.hpp"
#include "core/type/ptr.hpp"
#include "core/type/string.hpp"
#include "core/type/tile_cache.hpp"
#include "core/type/vector.hpp"
#include "core/uuid.hpp"
#include "core/viewport.hpp"
#include "core/vocabulary.hpp"

namespace tactile {

/// Represents a loaded map.
class MapDocument final : public Document {
 public:
  MapDocument(const Int2& tile_size, usize rows, usize columns);

  void update() override;

  void add_row();

  void add_column();

  void remove_row();

  void remove_column();

  void resize(usize rows, usize cols);

  void fix_tiles();

  void add_layer(LayerType type);

  void remove_layer(const UUID& layer_id);

  void duplicate_layer(const UUID& layer_id);

  void rename_layer(const UUID& layer_id, String name);

  void move_layer_up(const UUID& layer_id);

  void move_layer_down(const UUID& layer_id);

  void set_layer_opacity(const UUID& layer_id, float opacity);

  void set_layer_visible(const UUID& layer_id, bool visible);

  void register_stamp_sequence(const UUID& layer_id,
                               TileCache previous,
                               TileCache sequence);

  void register_eraser_sequence(const UUID& layer_id, TileCache previous);

  void flood(const UUID& layer_id, const TilePos& origin, TileID replacement);

  void add_rectangle(const UUID& layer_id, const Float2& pos, const Float2& size);

  void add_ellipse(const UUID& layer_id, const Float2& pos, const Float2& size);

  void add_point(const UUID& layer_id, const Float2& pos);

  void remove_object(const UUID& object_id);

  void move_object(const UUID& object_id, const Float2& previous, const Float2& updated);

  void set_object_visible(const UUID& object_id, bool visible);

  void set_object_name(const UUID& object_id, String name);

  void set_object_tag(const UUID& object_id, String tag);

  [[nodiscard]] auto get_object(const UUID& object_id) -> Shared<Object>;

  void set_tile_format_encoding(TileEncoding encoding);
  void set_tile_format_compression(TileCompression compression);

  void set_zlib_compression_level(int level);
  void set_zstd_compression_level(int level);

  void set_component_index(Shared<ComponentIndex> index) override;

  void set_name(String name) override;

  void set_path(Path path) override;

  [[nodiscard]] auto has_path() const -> bool override;

  [[nodiscard]] auto get_component_index() -> Shared<ComponentIndex> override;
  [[nodiscard]] auto view_component_index() const -> const ComponentIndex* override;

  [[nodiscard]] auto get_tools() -> ToolManager&;
  [[nodiscard]] auto get_tools() const -> const ToolManager&;

  [[nodiscard]] auto get_contexts() -> ContextManager& override;
  [[nodiscard]] auto get_contexts() const -> const ContextManager& override;

  [[nodiscard]] auto get_history() -> CommandStack& override;
  [[nodiscard]] auto get_history() const -> const CommandStack& override;

  [[nodiscard]] auto get_viewport() -> Viewport& override;
  [[nodiscard]] auto get_viewport() const -> const Viewport& override;

  [[nodiscard]] auto get_name() const -> const String& override;

  [[nodiscard]] auto get_path() const -> const Path& override;

  [[nodiscard]] auto get_type() const -> DocumentType override;

  [[nodiscard]] auto get_map_ptr() -> const Shared<Map>& { return mMap; }

  [[nodiscard]] auto get_map() -> Map& { return *mMap; }

  [[nodiscard]] auto get_map() const -> const Map& { return *mMap; }

 private:
  Shared<Map> mMap;
  ToolManager mTools;
  DocumentDelegate mDelegate;
};

}  // namespace tactile
