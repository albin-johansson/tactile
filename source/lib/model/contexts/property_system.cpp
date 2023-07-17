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

#include "property_system.hpp"

#include <utility>  // move

#include "common/debug/assert.hpp"

namespace tactile::sys {

void rename_property(Context& context, StringView current_name, String new_name)
{
  TACTILE_ASSERT(!context.props.contains(new_name));

  const auto iter = context.props.find(current_name);
  if (iter != context.props.end()) {
    auto property_value = iter->second;
    context.props.erase(iter);
    context.props[new_name] = std::move(property_value);
  }
}

}  // namespace tactile::sys