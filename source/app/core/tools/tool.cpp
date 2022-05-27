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

#include "tool.hpp"

namespace tactile {

void ATool::draw_gizmos(const DocumentModel&, IRenderer&, const MouseInfo&) const {}

void ATool::on_enabled(entt::registry&, entt::dispatcher&) {}

void ATool::on_disabled(entt::registry&, entt::dispatcher&) {}

void ATool::on_entered(entt::registry&, entt::dispatcher&) {}

void ATool::on_exited(entt::registry&, entt::dispatcher&) {}

void ATool::on_pressed(entt::registry&, entt::dispatcher&, const MouseInfo&) {}

void ATool::on_dragged(entt::registry&, entt::dispatcher&, const MouseInfo&) {}

void ATool::on_released(entt::registry&, entt::dispatcher&, const MouseInfo&) {}

auto ATool::is_available(const entt::registry&) const -> bool
{
  return true;
}

}  // namespace tactile