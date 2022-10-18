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

#include "core/ctx/context_manager.hpp"
#include "model/cmd/command_stack.hpp"
#include "model/cmd/comp/all.hpp"
#include "model/cmd/property/all.hpp"

namespace tactile {

void Document::define_component(String name)
{
  get_history().push<cmd::DefineComponent>(get_component_index(), std::move(name));
}

void Document::undef_component(const UUID& component_id)
{
  get_history().push<cmd::UndefComponent>(this, component_id);
}

void Document::rename_component(const UUID& component_id, String name)
{
  get_history().push<cmd::RenameComponent>(get_component_index(),
                                           component_id,
                                           std::move(name));
}

void Document::add_component_attribute(const UUID& component_id, String name)
{
  get_history().push<cmd::AddComponentAttr>(this, component_id, std::move(name));
}

void Document::remove_component_attribute(const UUID& component_id, String name)
{
  get_history().push<cmd::RemoveComponentAttr>(this, component_id, std::move(name));
}

void Document::rename_component_attribute(const UUID& component_id,
                                          String current,
                                          String updated)
{
  get_history().push<cmd::RenameComponentAttr>(this,
                                               component_id,
                                               std::move(current),
                                               std::move(updated));
}

void Document::duplicate_component_attribute(const UUID& component_id, String name)
{
  get_history().push<cmd::DuplicateComponentAttr>(this, component_id, std::move(name));
}

void Document::set_component_attribute_type(const UUID& component_id,
                                            String name,
                                            const AttributeType type)
{
  get_history().push<cmd::SetComponentAttrType>(this,
                                                component_id,
                                                std::move(name),
                                                type);
}

void Document::update_component(const UUID& component_id, String name, Attribute value)
{
  get_history().push<cmd::UpdateComponent>(get_component_index(),
                                           component_id,
                                           std::move(name),
                                           std::move(value));
}

void Document::attach_component(const UUID& context_id, const UUID& component_id)
{
  auto context = get_contexts().get_context(context_id);
  get_history().push<cmd::AttachComponent>(get_component_index(),
                                           std::move(context),
                                           component_id);
}

void Document::detach_component(const UUID& context_id, const UUID& component_id)
{
  auto context = get_contexts().get_context(context_id);
  get_history().push<cmd::DetachComponent>(std::move(context), component_id);
}

void Document::update_attached_component(const UUID& context_id,
                                         const UUID& component_id,
                                         String name,
                                         Attribute value)
{
  auto context = get_contexts().get_context(context_id);
  get_history().push<cmd::UpdateAttachedComponent>(std::move(context),
                                                   component_id,
                                                   std::move(name),
                                                   std::move(value));
}

void Document::reset_attached_component(const UUID& context_id, const UUID& component_id)
{
  auto context = get_contexts().get_context(context_id);
  get_history().push<cmd::ResetAttachedComponent>(get_component_index(),
                                                  std::move(context),
                                                  component_id);
}

void Document::add_property(const UUID& context_id, String name, const AttributeType type)
{
  auto context = get_contexts().get_context(context_id);
  get_history().push<cmd::AddProperty>(std::move(context), std::move(name), type);
}

void Document::remove_property(const UUID& context_id, String name)
{
  auto context = get_contexts().get_context(context_id);
  get_history().push<cmd::RemoveProperty>(std::move(context), std::move(name));
}

void Document::rename_property(const UUID& context_id, String current, String updated)
{
  auto context = get_contexts().get_context(context_id);
  get_history().push<cmd::RenameProperty>(std::move(context),
                                          std::move(current),
                                          std::move(updated));
}

void Document::update_property(const UUID& context_id, String name, Attribute value)
{
  auto context = get_contexts().get_context(context_id);
  get_history().push<cmd::UpdateProperty>(std::move(context),
                                          std::move(name),
                                          std::move(value));
}

void Document::change_property_type(const UUID& context_id,
                                    String name,
                                    const AttributeType type)
{
  auto context = get_contexts().get_context(context_id);
  get_history().push<cmd::ChangePropertyType>(std::move(context), std::move(name), type);
}

auto Document::is_map() const -> bool
{
  return get_type() == DocumentType::Map;
}

auto Document::is_tileset() const -> bool
{
  return get_type() == DocumentType::Tileset;
}

}  // namespace tactile