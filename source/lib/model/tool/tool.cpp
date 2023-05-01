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

#include "model/tool/tool.hpp"

namespace tactile {

void Tool::on_enabled(Model&, entt::dispatcher&) {}

void Tool::on_disabled(Model&, entt::dispatcher&) {}

void Tool::on_entered(Model&, entt::dispatcher&) {}

void Tool::on_exited(Model&, entt::dispatcher&) {}

void Tool::on_pressed(Model&, entt::dispatcher&, const MouseInfo&) {}

void Tool::on_dragged(Model&, entt::dispatcher&, const MouseInfo&) {}

void Tool::on_released(Model&, entt::dispatcher&, const MouseInfo&) {}

auto Tool::is_available(const Model&) const -> bool
{
  return true;
}

}  // namespace tactile