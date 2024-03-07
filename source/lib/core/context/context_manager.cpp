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

#include "context_manager.hpp"

#include <utility>  // move

#include "common/util/assoc.hpp"
#include "core/context/context_info.hpp"
#include "tactile/core/debug/exception.hpp"
#include "tactile/core/debug/generic_error.hpp"

namespace tactile {

ContextManager::ContextManager(const UUID& root_ctx_id)
    : mRootContextId {root_ctx_id}
{
}

void ContextManager::add_context(Shared<Context> context)
{
  const auto ctx_id = context->get_uuid();
  mContexts[ctx_id] = std::move(context);
}

auto ContextManager::remove_context(const UUID& ctx_id) -> Result<void>
{
  if (const auto iter = mContexts.find(ctx_id); iter != mContexts.end()) [[likely]] {
    mContexts.erase(iter);

    if (ctx_id == mActiveContextId) {
      mActiveContextId = mRootContextId;
    }

    return kOK;
  }
  else {
    return unexpected(make_error(GenericError::kInvalidParam));
  }
}

auto ContextManager::select_context(const UUID& ctx_id) -> Result<void>
{
  if (has_context(ctx_id)) [[likely]] {
    mActiveContextId = ctx_id;
    return kOK;
  }
  else {
    return unexpected(make_error(GenericError::kInvalidParam));
  }
}

auto ContextManager::get_context_ptr(const UUID& ctx_id) -> const Shared<Context>&
{
  return lookup_in(mContexts, ctx_id);
}

auto ContextManager::get_context(const UUID& ctx_id) -> Context&
{
  return *lookup_in(mContexts, ctx_id);
}

auto ContextManager::get_context(const UUID& ctx_id) const -> const Context&
{
  return *lookup_in(mContexts, ctx_id);
}

auto ContextManager::has_context(const UUID& ctx_id) const -> bool
{
  return mContexts.find(ctx_id) != mContexts.end();
}

auto ContextManager::context_count() const -> usize
{
  return mContexts.size();
}

auto ContextManager::get_active_context() -> Context&
{
  return *lookup_in(mContexts, mActiveContextId);
}

auto ContextManager::get_active_context() const -> const Context&
{
  return *lookup_in(mContexts, mActiveContextId);
}

auto ContextManager::on_undef_comp(const UUID& component_id) -> HashMap<UUID, Component>
{
  HashMap<UUID, Component> removed;

  for (auto& [ctx_id, context]: mContexts) {
    auto& ctx = context->get_ctx();
    if (ctx.has_component(component_id)) {
      removed.try_emplace(ctx_id, ctx.detach_component(component_id).value());
    }
  }

  return removed;
}

void ContextManager::on_new_component_attr(const UUID& component_id,
                                           const String& name,
                                           const Attribute& value)
{
  on_component_update(component_id,
                      [&](Component& component) { component.add_attr(name, value); });
}

void ContextManager::on_removed_component_attr(const UUID& component_id, StringView name)
{
  on_component_update(component_id,
                      [name](Component& component) { component.remove_attr(name); });
}

void ContextManager::on_renamed_component_attr(const UUID& component_id,
                                               StringView old_name,
                                               const String& new_name)
{
  on_component_update(component_id, [old_name, &new_name](Component& component) {
    component.rename_attr(old_name, new_name);
  });
}

auto ContextManager::on_changed_component_attr_type(const UUID& component_id,
                                                    StringView name,
                                                    const AttributeType type)
    -> HashMap<UUID, Attribute>
{
  HashMap<UUID, Attribute> attributes;

  for (auto& [ctx_id, context]: mContexts) {
    if (auto* component = context->get_ctx().find_component(component_id)) {
      attributes[ctx_id] = component->get_attr(name);

      component->remove_attr(name);
      component->add_attr(String {name}, Attribute {type});
    }
  }

  return attributes;
}

void ContextManager::on_component_update(const UUID& component_id,
                                         const ComponentFunc& func)
{
  for (auto& [contextId, context]: mContexts) {
    if (auto* component = context->get_ctx().find_component(component_id)) {
      func(*component);
    }
  }
}

}  // namespace tactile
