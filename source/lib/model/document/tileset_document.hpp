/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include "core/tile/tileset.hpp"
#include "core/tile/tileset_info.hpp"
#include "model/document/document.hpp"
#include "model/document/document_delegate.hpp"
#include "tactile/base/container/path.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/base/int.hpp"
#include "tactile/base/util/chrono.hpp"
#include "tactile/core/util/uuid.hpp"

namespace tactile {

/// Represents a loaded tileset.
///
/// The document model holds a tileset document for each tileset attached to a map.
/// However, tileset documents can be shared across multiple maps as long as they are
/// external.
class TilesetDocument final : public Document {
 public:
  explicit TilesetDocument(TilesetInfo info, const UUID& id = UUID::generate());

  void update() override;

  /// Executes a command that renames the tileset.
  void rename_tileset(String name);

  /// Executes a command that removes a tile animation in the tileset.
  void delete_animation(TileIndex tile_index);

  /// Executes a command that adds a frame to an animation in the tileset.
  void add_animation_frame(TileIndex animated_tile_index,
                           TileIndex frame_tile_index,
                           Milliseconds frame_duration);

  /// Executes a command that removes an animation frame in the tileset.
  void remove_animation_frame(TileIndex tile_index, usize frame_index);

  /// Executes a command that shifts an animation frame forwards in the tileset.
  void move_animation_frame_forwards(TileIndex tile_index, usize frame_index);

  /// Executes a command that shifts an animation frame backwards in the tileset.
  void move_animation_frame_backwards(TileIndex tile_index, usize frame_index);

  /// Executes a command that sets the duration of an animation frame in the tileset.
  void set_animation_frame_duration(TileIndex tile_index,
                                    usize frame_index,
                                    Milliseconds frame_duration);

  /// Executes a command that renames a tile in the tileset.
  void rename_tile(TileIndex tile_index, String name);

  void set_component_index(Shared<ComponentIndex> index) override;

  void set_name(String name) override;

  void set_path(Path path) override;

  [[nodiscard]] auto has_path() const -> bool override;

  [[nodiscard]] auto get_component_index_ptr() -> Shared<ComponentIndex> override;
  [[nodiscard]] auto find_component_index() const -> const ComponentIndex* override;

  [[nodiscard]] auto get_contexts() -> ContextManager& override;
  [[nodiscard]] auto get_contexts() const -> const ContextManager& override;

  [[nodiscard]] auto get_history() -> CommandStack& override;
  [[nodiscard]] auto get_history() const -> const CommandStack& override;

  [[nodiscard]] auto get_viewport() -> Viewport& override;
  [[nodiscard]] auto get_viewport() const -> const Viewport& override;

  [[nodiscard]] auto get_name() const -> const String& override;

  [[nodiscard]] auto get_path() const -> const Path& override;

  [[nodiscard]] auto get_type() const -> DocumentType override;

  [[nodiscard]] auto get_tileset_ptr() -> Shared<Tileset> { return mTileset; }

  [[nodiscard]] auto get_tileset() -> Tileset& { return *mTileset; }
  [[nodiscard]] auto get_tileset() const -> const Tileset& { return *mTileset; }

 private:
  Shared<Tileset> mTileset;
  DocumentDelegate mDelegate;
};

}  // namespace tactile
