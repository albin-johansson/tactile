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

#include "core/events/dispatcher.hpp"
#include "tactile/core/common/prelude.hpp"

namespace tactile {

TACTILE_FWD_DECLARE_CLASS(Registry)
TACTILE_FWD_DECLARE_STRUCT(ViewportMouseInfo)

class Tool {
 public:
  virtual ~Tool() noexcept = default;

  virtual void reset() = 0;

  virtual void on_activated(Registry& registry, Dispatcher& dispatcher);

  virtual void on_deactivated(Registry& registry, Dispatcher& dispatcher);

  virtual void on_mouse_entered(Registry& registry, Dispatcher& dispatcher);

  virtual void on_mouse_exited(Registry& registry, Dispatcher& dispatcher);

  virtual void on_mouse_pressed(Registry& registry,
                                Dispatcher& dispatcher,
                                const ViewportMouseInfo& mouse);

  virtual void on_mouse_dragged(Registry& registry,
                                Dispatcher& dispatcher,
                                const ViewportMouseInfo& mouse);

  virtual void on_mouse_released(Registry& registry,
                                 Dispatcher& dispatcher,
                                 const ViewportMouseInfo& mouse);

  [[nodiscard]] virtual auto is_available(const Registry& registry) const -> bool = 0;
};

}  // namespace tactile
