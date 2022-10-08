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

#include "core/ctx/context_info.hpp"
#include "core/util/assoc.hpp"
#include "misc/panic.hpp"

namespace tactile {

ContextManager::ContextManager(const UUID& rootContextId)
    : mRootContextId {rootContextId}
{
}

void ContextManager::add_context(Shared<Context> context)
{
  const auto id = context->uuid();
  mContexts[id] = std::move(context);
}

void ContextManager::erase(const UUID& contextId)
{
  if (const auto iter = mContexts.find(contextId); iter != mContexts.end()) [[likely]] {
    mContexts.erase(iter);
    if (contextId == mActiveContextId) {
      mActiveContextId = mRootContextId;
    }
  }
  else {
    throw TactileError {"Tried to remove non-existent context!"};
  }
}

void ContextManager::select(const UUID& contextId)
{
  if (contains(contextId)) [[likely]] {
    mActiveContextId = contextId;
  }
  else {
    throw TactileError {"Tried to select non-existent context!"};
  }
}

auto ContextManager::get_context(const UUID& contextId) -> const Shared<Context>&
{
  return lookup_in(mContexts, contextId);
}

auto ContextManager::at(const UUID& contextId) -> Context&
{
  return *lookup_in(mContexts, contextId);
}

auto ContextManager::at(const UUID& contextId) const -> const Context&
{
  return *lookup_in(mContexts, contextId);
}

auto ContextManager::contains(const UUID& contextId) const -> bool
{
  return mContexts.find(contextId) != mContexts.end();
}

auto ContextManager::size() const -> usize
{
  return mContexts.size();
}

auto ContextManager::active_context() -> Context&
{
  return *lookup_in(mContexts, mActiveContextId);
}

auto ContextManager::active_context() const -> const Context&
{
  return *lookup_in(mContexts, mActiveContextId);
}

auto ContextManager::on_undef_comp(const UUID& componentId) -> HashMap<UUID, Component>
{
  HashMap<UUID, Component> removed;

  for (auto& [contextId, context] : mContexts) {
    auto& comps = context->ctx().comps();
    if (comps.contains(componentId)) {
      removed.try_emplace(contextId, comps.erase(componentId));
    }
  }

  return removed;
}

void ContextManager::on_new_component_attr(const UUID& componentId,
                                           const String& name,
                                           const Attribute& value)
{
  on_component_update(componentId,
                      [&](Component& component) { component.add_attr(name, value); });
}

void ContextManager::on_removed_component_attr(const UUID& componentId, StringView name)
{
  on_component_update(componentId,
                      [name](Component& component) { component.remove_attr(name); });
}

void ContextManager::on_renamed_component_attr(const UUID& componentId,
                                               StringView oldName,
                                               const String& newName)
{
  on_component_update(componentId, [oldName, &newName](Component& component) {
    component.rename_attr(oldName, newName);
  });
}

auto ContextManager::on_changed_component_attr_type(const UUID& componentId,
                                                    StringView name,
                                                    const AttributeType type)
    -> HashMap<UUID, Attribute>
{
  HashMap<UUID, Attribute> attributes;

  for (auto& [contextId, context] : mContexts) {
    auto& comps = context->ctx().comps();
    if (comps.contains(componentId)) {
      auto& comp = comps.at(componentId);
      attributes[contextId] = comp.get_attr(name);

      comp.remove_attr(name);
      comp.add_attr(String {name}, Attribute {type});
    }
  }

  return attributes;
}

void ContextManager::on_component_update(const UUID& componentId,
                                         const ComponentFunc& func)
{
  for (auto& [contextId, context] : mContexts) {
    auto& comps = context->ctx().comps();
    if (auto* component = comps.try_get(componentId)) {
      func(*component);
    }
  }
}

}  // namespace tactile
