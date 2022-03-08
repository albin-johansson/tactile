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

#include "component_name_dialog.hpp"

#include "core/systems/component_system.hpp"
#include "editor/model.hpp"

namespace tactile {

auto component_name_dialog::validate(const document_model& model,
                                     std::string_view input) const -> bool
{
  const auto& registry = model.get_active_registry();
  return !input.empty() && !sys::is_component_name_taken(registry, input);
}

}  // namespace tactile