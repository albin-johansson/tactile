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

#include "document.hpp"

#include <utility>  // move

#include "core/commands/command_stack.hpp"
#include "core/commands/components/attach_component_cmd.hpp"

namespace tactile {

void ADocument::set_path(std::filesystem::path path)
{
  mPath = std::move(path);
}

void ADocument::attach_component(const UUID& contextId, const UUID& componentId)
{
  get_history().push<AttachComponentCmd>(this, contextId, componentId);
}

void ADocument::set_component_index(Shared<core::ComponentIndex> index)
{
  mComponentIndex = std::move(index);
}

auto ADocument::get_component_index() -> Shared<core::ComponentIndex>
{
  return mComponentIndex;
}

auto ADocument::get_component_index() const -> Shared<const core::ComponentIndex>
{
  return mComponentIndex;
}

auto ADocument::has_path() const -> bool
{
  return mPath.has_value();
}

auto ADocument::get_path() const -> const std::filesystem::path&
{
  return mPath.value();
}

auto ADocument::get_history() -> CommandStack&
{
  return mCommands;
}

auto ADocument::get_history() const -> const CommandStack&
{
  return mCommands;
}

auto ADocument::is_map() const -> bool
{
  return get_type() == DocumentType::Map;
}

auto ADocument::is_tileset() const -> bool
{
  return get_type() == DocumentType::Tileset;
}

}  // namespace tactile