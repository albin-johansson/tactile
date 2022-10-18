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

ContextManager::ContextManager(const UUID& root_context_id)
    : mRootContextId {root_context_id}
{
}

void ContextManager::add_context(Shared<Context> context)
{
  const auto id = context->uuid();
  mContexts[id] = std::move(context);
}

void ContextManager::erase(const UUID& context_id)
{
  if (const auto iter = mContexts.find(context_id); iter != mContexts.end()) [[likely]] {
    mContexts.erase(iter);
    if (context_id == mActiveContextId) {
      mActiveContextId = mRootContextId;
    }
  }
  else {
    throw TactileError {"Tried to remove non-existent context!"};
  }
}

void ContextManager::select(const UUID& context_id)
{
  if (contains(context_id)) [[likely]] {
    mActiveContextId = context_id;
  }
  else {
    throw TactileError {"Tried to select non-existent context!"};
  }
}

auto ContextManager::get_context(const UUID& context_id) -> const Shared<Context>&
{
  return lookup_in(mContexts, context_id);
}

auto ContextManager::at(const UUID& context_id) -> Context&
{
  return *lookup_in(mContexts, context_id);
}

auto ContextManager::at(const UUID& context_id) const -> const Context&
{
  return *lookup_in(mContexts, context_id);
}

auto ContextManager::contains(const UUID& context_id) const -> bool
{
  return mContexts.find(context_id) != mContexts.end();
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

auto ContextManager::on_undef_comp(const UUID& component_id) -> HashMap<UUID, Component>
{
  HashMap<UUID, Component> removed;

  for (auto& [contextId, context]: mContexts) {
    auto& comps = context->ctx().comps();
    if (comps.contains(component_id)) {
      removed.try_emplace(contextId, comps.erase(component_id));
    }
  }

  return removed;
}

void ContextManager::on_new_component_attr(const UUID& component_id,
                                           const String& name,
                                           const Attribute& value)
{
  on_component_update(component_id,
                      [&](Component& component) { component.add(name, value); });
}

void ContextManager::on_removed_component_attr(const UUID& component_id, StringView name)
{
  on_component_update(component_id,
                      [name](Component& component) { component.remove(name); });
}

void ContextManager::on_renamed_component_attr(const UUID& component_id,
                                               StringView old_name,
                                               const String& new_name)
{
  on_component_update(component_id, [old_name, &new_name](Component& component) {
    component.rename(old_name, new_name);
  });
}

auto ContextManager::on_changed_component_attr_type(const UUID& component_id,
                                                    StringView name,
                                                    const AttributeType type)
    -> HashMap<UUID, Attribute>
{
  HashMap<UUID, Attribute> attributes;

  for (auto& [contextId, context]: mContexts) {
    auto& comps = context->ctx().comps();
    if (comps.contains(component_id)) {
      auto& comp = comps.at(component_id);
      attributes[contextId] = comp.at(name);

      comp.remove(name);
      comp.add(String {name}, Attribute {type});
    }
  }

  return attributes;
}

void ContextManager::on_component_update(const UUID& component_id,
                                         const ComponentFunc& func)
{
  for (auto& [contextId, context]: mContexts) {
    auto& comps = context->ctx().comps();
    if (auto* component = comps.try_get(component_id)) {
      func(*component);
    }
  }
}

}  // namespace tactile
