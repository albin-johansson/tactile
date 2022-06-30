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
#include "core/commands/properties/add_property_cmd.hpp"
#include "core/commands/properties/change_property_type_cmd.hpp"
#include "core/commands/properties/remove_property_cmd.hpp"
#include "core/commands/properties/rename_property_cmd.hpp"
#include "core/commands/properties/update_property_cmd.hpp"

namespace tactile {

void ADocument::attach_component(const UUID& contextId, const UUID& componentId)
{
  auto context = get_context(contextId);
  get_history().push<AttachComponentCmd>(mComponentIndex,
                                         std::move(context),
                                         componentId);
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

void ADocument::add_property(const UUID&         contextId,
                             std::string         name,
                             const AttributeType type)
{
  auto context = get_context(contextId);
  get_history().push<AddPropertyCmd>(std::move(context), std::move(name), type);
}

void ADocument::remove_property(const UUID& contextId, std::string name)
{
  auto context = get_context(contextId);
  get_history().push<RemovePropertyCmd>(std::move(context), std::move(name));
}

void ADocument::rename_property(const UUID& contextId,
                                std::string current,
                                std::string updated)
{
  auto context = get_context(contextId);
  get_history().push<RenamePropertyCmd>(std::move(context),
                                        std::move(current),
                                        std::move(updated));
}

void ADocument::update_property(const UUID& contextId, std::string name, Attribute value)
{
  auto context = get_context(contextId);
  get_history().push<UpdatePropertyCmd>(std::move(context),
                                        std::move(name),
                                        std::move(value));
}

void ADocument::change_property_type(const UUID&         contextId,
                                     std::string         name,
                                     const AttributeType type)
{
  auto context = get_context(contextId);
  get_history().push<ChangePropertyTypeCmd>(std::move(context), std::move(name), type);
}

void ADocument::set_path(std::filesystem::path path)
{
  mPath = std::move(path);
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

void ADocument::select_context(const UUID& contextId)
{
  mActiveContext = contextId;
}

auto ADocument::active_context_id() const -> const UUID&
{
  return mActiveContext;
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