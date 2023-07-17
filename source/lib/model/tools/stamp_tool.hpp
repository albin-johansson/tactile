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

#include "common/macros.hpp"
#include "common/tile_pos.hpp"
#include "common/type/dispatcher.hpp"
#include "common/type/ecs.hpp"
#include "common/type/maybe.hpp"
#include "common/type/tile_cache.hpp"
#include "model/maps/map_components.hpp"
#include "model/model.hpp"
#include "model/tools/tool.hpp"
#include "model/viewports/viewport_components.hpp"

namespace tactile {

TACTILE_FWD_DECLARE_STRUCT(Map)

class StampTool final : public Tool {
 public:
  void reset() override;

  void on_deactivated(Model& model, Dispatcher& dispatcher) override;

  void on_mouse_exited(Model& model, Dispatcher& dispatcher) override;

  void on_mouse_pressed(Model& model,
                        Dispatcher& dispatcher,
                        const ViewportMouseInfo& mouse) override;

  void on_mouse_dragged(Model& model,
                        Dispatcher& dispatcher,
                        const ViewportMouseInfo& mouse) override;

  void on_mouse_released(Model& model,
                         Dispatcher& dispatcher,
                         const ViewportMouseInfo& mouse) override;

  [[nodiscard]] auto is_available(const Model& model) const -> bool override;

 private:
  TileCache mOldState;
  TileCache mNewState;
  Maybe<TilePos> mLastChangedPos;
  bool mIsRandom {false};

  void _update_sequence(Model& model, const TilePos& mouse_pos);
  void _update_normal_sequence(Model& model, const Map& map, const TilePos& mouse_pos);
  void _update_random_sequence(Model& model, const Map& map, const TilePos& mouse_pos);
  void _try_end_sequence(Model& model, Dispatcher& dispatcher);

  [[nodiscard]] auto _behaves_as_if_random(const Model& model, const Map& map) const
      -> bool;
};

}  // namespace tactile

namespace tactile::sys {

[[nodiscard, deprecated]] auto is_stamp_tool_available(const Model& model) -> bool;

[[nodiscard]] auto is_stamp_tool_randomizer_possible(const Model& model, const Map& map)
    -> bool;

}  // namespace tactile::sys
