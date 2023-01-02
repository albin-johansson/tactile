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

#include "context_info.hpp"

#include <utility>  // move

#include "common/util/assoc.hpp"

namespace tactile {

ContextInfo::ContextInfo(const UUID& id)
    : mId {id}
{
}

void ContextInfo::set_name(String name)
{
  mName = std::move(name);
}

void ContextInfo::each_component(const ComponentVisitor& visitor) const
{
  for (const auto& [component_id, component]: mComponents) {
    visitor(component_id, component);
  }
}

auto ContextInfo::attach_component(Component component) -> Result
{
  const auto component_id = component.definition_id();
  auto [iter, did_insert] = mComponents.try_emplace(component_id, std::move(component));
  return did_insert ? success : failure;
}

auto ContextInfo::detach_component(const UUID& component_id) -> Maybe<Component>
{
  if (const auto iter = mComponents.find(component_id); iter != mComponents.end()) {
    auto removed_component = iter->second;
    mComponents.erase(iter);
    return removed_component;
  }

  return nothing;
}

auto ContextInfo::get_component(const UUID& component_id) -> Component&
{
  return lookup_in(mComponents, component_id);
}

auto ContextInfo::get_component(const UUID& component_id) const -> const Component&
{
  return lookup_in(mComponents, component_id);
}

auto ContextInfo::find_component(const UUID& component_id) -> Component*
{
  if (const auto iter = mComponents.find(component_id); iter != mComponents.end()) {
    return &iter->second;
  }
  else {
    return nullptr;
  }
}

auto ContextInfo::find_component(const UUID& component_id) const -> const Component*
{
  if (const auto iter = mComponents.find(component_id); iter != mComponents.end()) {
    return &iter->second;
  }
  else {
    return nullptr;
  }
}

auto ContextInfo::has_component(const UUID& component_id) const -> bool
{
  return mComponents.contains(component_id);
}

auto ContextInfo::component_count() const -> usize
{
  return mComponents.size();
}

auto ContextInfo::clone() const -> ContextInfo
{
  ContextInfo other {*this};
  other.mId = make_uuid();
  return other;
}

}  // namespace tactile
