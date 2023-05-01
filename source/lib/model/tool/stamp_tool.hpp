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

#include "common/type/maybe.hpp"
#include "common/type/tile_cache.hpp"
#include "core/layer.hpp"
#include "core/tileset.hpp"
#include "model/tool/tool.hpp"

namespace tactile {

/// A tool used to draw (or "stamp") selected tileset tiles to tile layers.
///
/// The stamp tool, in its default mode, will simply stamp the selected tiles in the
/// active tileset to tile layers. It also has a "random" mode, where a single tile is
/// randomly picked from the tile selection, and subsequently stamped to the tile layer.
///
/// Changes are applied to the target tile layer immediately, and later submitted
/// as a command when the user ends the sequence.
///
/// \todo Right-click should erase tiles.
class StampTool final : public Tool {
 public:
  void accept(ToolVisitor& visitor) const override;

  void on_disabled(Model& model, Dispatcher& dispatcher) override;

  void on_exited(Model& model, Dispatcher& dispatcher) override;

  void on_pressed(Model& model, Dispatcher& dispatcher, const MouseInfo& mouse) override;

  void on_dragged(Model& model, Dispatcher& dispatcher, const MouseInfo& mouse) override;

  void on_released(Model& model, Dispatcher& dispatcher, const MouseInfo& mouse) override;

  void set_random(bool random);

  [[nodiscard]] auto is_random() const -> bool;

  [[nodiscard]] auto behaves_as_if_random(const Model& model, Entity map_entity) const
      -> bool;

  [[nodiscard]] auto is_available(const Model& model) const -> bool override;

  [[nodiscard]] auto get_type() const -> ToolType override { return ToolType::Stamp; }

 private:
  TileCache mPrevious;  ///< Previous tile state.
  TileCache mCurrent;   ///< The current stamp sequence.
  Maybe<TilePos> mLastChangedPos;
  bool mRandomMode {};

  void update_sequence(Model& model, const TilePos& cursor);

  void update_sequence_normal(Model& model,
                              TileLayer& tile_layer,
                              const AttachedTileset& attached_tileset,
                              const TilePos& cursor);

  void update_sequence_random(Model& model,
                              TileLayer& tile_layer,
                              const AttachedTileset& attached_tileset,
                              const TilePos& cursor);

  void maybe_emit_event(const Model& model, Dispatcher& dispatcher);

  [[nodiscard]] auto is_usable(const Model& model) const -> bool;
};

}  // namespace tactile
