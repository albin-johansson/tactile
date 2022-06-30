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

#include <entt/fwd.hpp>

#include "core/attribute.hpp"
#include "core/fwd.hpp"

namespace tactile::ui {

void update_property_dock(const DocumentModel& model, entt::dispatcher& dispatcher);

void show_property_creation_dialog();

void show_rename_property_dialog(const std::string& name);

void show_change_property_type_dialog(std::string name, AttributeType type);

[[nodiscard]] auto is_property_dock_focused() -> bool;

}  // namespace tactile::ui
