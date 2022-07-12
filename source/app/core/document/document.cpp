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

#include "core/cmd/command_stack.hpp"
#include "core/cmd/comp/all.hpp"
#include "core/cmd/property/all.hpp"
#include "core/context/context_manager.hpp"

namespace tactile {

void ADocument::set_component_index(Shared<ComponentIndex> index)
{
  mComponentIndex = std::move(index);
}

auto ADocument::get_component_index() -> Shared<ComponentIndex>
{
  return mComponentIndex;
}

auto ADocument::get_component_index() const -> Shared<const ComponentIndex>
{
  return mComponentIndex;
}

void ADocument::define_component(std::string name)
{
  get_history().exec<DefineComponentCmd>(mComponentIndex, std::move(name));
}

void ADocument::undef_component(const UUID& componentId)
{
  get_history().exec<UndefComponentCmd>(this, componentId);
}

void ADocument::rename_component(const UUID& componentId, std::string name)
{
  get_history().exec<RenameComponentCmd>(mComponentIndex, componentId, std::move(name));
}

void ADocument::add_component_attribute(const UUID& componentId, std::string name)
{
  get_history().exec<AddComponentAttrCmd>(this, componentId, std::move(name));
}

void ADocument::remove_component_attribute(const UUID& componentId, std::string name)
{
  get_history().exec<RemoveComponentAttrCmd>(this, componentId, std::move(name));
}

void ADocument::rename_component_attribute(const UUID& componentId,
                                           std::string current,
                                           std::string updated)
{
  get_history().exec<RenameComponentAttrCmd>(this,
                                             componentId,
                                             std::move(current),
                                             std::move(updated));
}

void ADocument::duplicate_component_attribute(const UUID& componentId, std::string name)
{
  get_history().exec<DuplicateComponentAttrCmd>(this, componentId, std::move(name));
}

void ADocument::set_component_attribute_type(const UUID&         componentId,
                                             std::string         name,
                                             const AttributeType type)
{
  get_history().exec<SetComponentAttrTypeCmd>(this, componentId, std::move(name), type);
}

void ADocument::update_component(const UUID& componentId,
                                 std::string name,
                                 Attribute   value)
{
  get_history().exec<UpdateComponentCmd>(mComponentIndex,
                                         componentId,
                                         std::move(name),
                                         std::move(value));
}

void ADocument::attach_component(const UUID& contextId, const UUID& componentId)
{
  auto context = get_contexts().get_context(contextId);
  get_history().exec<AttachComponentCmd>(mComponentIndex,
                                         std::move(context),
                                         componentId);
}

void ADocument::detach_component(const UUID& contextId, const UUID& componentId)
{
  auto context = get_contexts().get_context(contextId);
  get_history().exec<DetachComponentCmd>(std::move(context), componentId);
}

void ADocument::update_attached_component(const UUID& contextId,
                                          const UUID& componentId,
                                          std::string name,
                                          Attribute   value)
{
  auto context = get_contexts().get_context(contextId);
  get_history().exec<UpdateAttachedComponentCmd>(std::move(context),
                                                 componentId,
                                                 std::move(name),
                                                 std::move(value));
}

void ADocument::reset_attached_component(const UUID& contextId, const UUID& componentId)
{
  auto context = get_contexts().get_context(contextId);
  get_history().exec<ResetAttachedComponentCmd>(mComponentIndex,
                                                std::move(context),
                                                componentId);
}

void ADocument::add_property(const UUID&         contextId,
                             std::string         name,
                             const AttributeType type)
{
  auto context = get_contexts().get_context(contextId);
  get_history().exec<AddPropertyCmd>(std::move(context), std::move(name), type);
}

void ADocument::remove_property(const UUID& contextId, std::string name)
{
  auto context = get_contexts().get_context(contextId);
  get_history().exec<RemovePropertyCmd>(std::move(context), std::move(name));
}

void ADocument::rename_property(const UUID& contextId,
                                std::string current,
                                std::string updated)
{
  auto context = get_contexts().get_context(contextId);
  get_history().exec<RenamePropertyCmd>(std::move(context),
                                        std::move(current),
                                        std::move(updated));
}

void ADocument::update_property(const UUID& contextId, std::string name, Attribute value)
{
  auto context = get_contexts().get_context(contextId);
  get_history().exec<UpdatePropertyCmd>(std::move(context),
                                        std::move(name),
                                        std::move(value));
}

void ADocument::change_property_type(const UUID&         contextId,
                                     std::string         name,
                                     const AttributeType type)
{
  auto context = get_contexts().get_context(contextId);
  get_history().exec<ChangePropertyTypeCmd>(std::move(context), std::move(name), type);
}

void ADocument::set_path(fs::path path)
{
  mPath = std::move(path);
}

auto ADocument::has_path() const -> bool
{
  return mPath.has_value();
}

auto ADocument::get_path() const -> const fs::path&
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