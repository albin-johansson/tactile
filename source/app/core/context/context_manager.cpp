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

#include "context_manager.hpp"

#include <utility>  // move

#include "core/comp/component_bundle.hpp"
#include "misc/panic.hpp"

namespace tactile {

void ContextManager::add_context(Shared<IContext> context)
{
  const auto id = context->get_uuid();
  mContexts[id] = std::move(context);
}

void ContextManager::remove_context(const UUID& id)
{
  if (const auto iter = mContexts.find(id); iter != mContexts.end()) {
    mContexts.erase(iter);
  }
  else {
    throw TactileError {"Tried to remove non-existent context!"};
  }
}

auto ContextManager::get_context(const UUID& id) -> const Shared<IContext>&
{
  return lookup_in(mContexts, id);
}

auto ContextManager::view_context(const UUID& id) const -> const IContext&
{
  return *lookup_in(mContexts, id);
}

auto ContextManager::has_context(const UUID& id) const -> bool
{
  return mContexts.contains(id);
}

auto ContextManager::context_count() const -> usize
{
  return mContexts.size();
}

auto ContextManager::on_undef_comp(const UUID& componentId) -> HashMap<UUID, Component>
{
  HashMap<UUID, Component> removed;

  for (auto& [contextId, context] : mContexts) {
    auto& comps = context->get_comps();
    if (comps.contains(componentId)) {
      removed.try_emplace(contextId, comps.erase(componentId));
    }
  }

  return removed;
}

void ContextManager::on_new_component_attr(const UUID&        componentId,
                                           const std::string& name,
                                           const Attribute&   value)
{
  on_component_update(componentId,
                      [&](Component& component) { component.add_attr(name, value); });
}

void ContextManager::on_removed_component_attr(const UUID&      componentId,
                                               std::string_view name)
{
  on_component_update(componentId,
                      [name](Component& component) { component.remove_attr(name); });
}

void ContextManager::on_renamed_component_attr(const UUID&        componentId,
                                               std::string_view   oldName,
                                               const std::string& newName)
{
  on_component_update(componentId, [oldName, &newName](Component& component) {
    component.rename_attr(oldName, newName);
  });
}

auto ContextManager::on_changed_component_attr_type(const UUID&         componentId,
                                                    std::string_view    name,
                                                    const AttributeType type)
    -> HashMap<UUID, Attribute>
{
  HashMap<UUID, Attribute> attributes;

  for (auto& [contextId, context] : mContexts) {
    auto& comps = context->get_comps();
    if (comps.contains(componentId)) {
      auto& comp = comps.at(componentId);
      attributes[contextId] = comp.get_attr(name);

      comp.remove_attr(name);
      comp.add_attr(std::string {name}, Attribute {type});
    }
  }

  return attributes;
}

void ContextManager::on_component_update(const UUID&          componentId,
                                         const ComponentFunc& func)
{
  for (auto& [contextId, context] : mContexts) {
    auto& comps = context->get_comps();
    if (auto* component = comps.try_get(componentId)) {
      func(*component);
    }
  }
}

}  // namespace tactile
