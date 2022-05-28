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

#include <centurion/mouse.hpp>
#include <entt/fwd.hpp>

#include "core/common/macros.hpp"
#include "editor/ui/rendering/render_info.hpp"

namespace tactile::ui {

class ViewportArea
{
 public:
  TACTILE_DEFAULT_COPY(ViewportArea);
  TACTILE_DEFAULT_MOVE(ViewportArea);

  ViewportArea() noexcept = default;

  virtual ~ViewportArea() noexcept = default;

  [[nodiscard]] auto get_size() const -> ImVec2;

 protected:
  void check_mouse_events(const entt::registry& registry, entt::dispatcher& dispatcher);

  [[nodiscard]] auto contains_mouse() const -> bool;

  virtual void on_clicked(cen::mouse_button, const entt::registry&, entt::dispatcher&) {}

  virtual void on_dragged(cen::mouse_button, const entt::registry&, entt::dispatcher&) {}

  virtual void on_released(cen::mouse_button, const entt::registry&, entt::dispatcher&) {}

 private:
  bool mContainsMouse{};

  void check_clicked(const entt::registry& registry, entt::dispatcher& dispatcher);

  void check_dragged(const entt::registry& registry, entt::dispatcher& dispatcher);

  void check_released(const entt::registry& registry, entt::dispatcher& dispatcher);
};

}  // namespace tactile::ui