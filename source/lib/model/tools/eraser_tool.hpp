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

#include "common/tile_pos.hpp"
#include "common/type/tile_cache.hpp"
#include "model/tools/tool.hpp"

namespace tactile {

class EraserTool final : public Tool {
 public:
  void reset() override;

  void on_deactivated(Registry& registry, Dispatcher& dispatcher) override;

  void on_mouse_exited(Registry& registry, Dispatcher& dispatcher) override;

  void on_mouse_pressed(Registry& registry,
                        Dispatcher& dispatcher,
                        const ViewportMouseInfo& mouse) override;

  void on_mouse_dragged(Registry& registry,
                        Dispatcher& dispatcher,
                        const ViewportMouseInfo& mouse) override;

  void on_mouse_released(Registry& registry,
                         Dispatcher& dispatcher,
                         const ViewportMouseInfo& mouse) override;

  [[nodiscard]] auto is_available(const Registry& registry) const -> bool override;

 private:
  TileCache mOldState;

  void _update_sequence(Registry& registry, const TilePos& mouse_pos);
  void _try_end_sequence(Registry& registry);
};

}  // namespace tactile
