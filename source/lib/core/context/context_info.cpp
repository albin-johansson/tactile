// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "context_info.hpp"

#include <utility>  // move

#include "common/util/assoc.hpp"
#include "tactile/core/debug/generic_error.hpp"

namespace tactile {

ContextInfo::ContextInfo(const UUID& id)
    : mId {id}
{
}

void ContextInfo::set_name(String name)
{
  mName = std::move(name);
}

void ContextInfo::each_property(const PropertyVisitor& visitor) const
{
  for (const auto& [property_name, property_value]: mProperties) {
    visitor(property_name, property_value);
  }
}

void ContextInfo::each_component(const ComponentVisitor& visitor) const
{
  for (const auto& [component_id, component]: mComponents) {
    visitor(component_id, component);
  }
}

auto ContextInfo::add_property(String name, Attribute value) -> Result<void>
{
  auto [iter, did_insert] = mProperties.try_emplace(std::move(name), std::move(value));
  return did_insert ? kOK : unexpected(make_error(GenericError::kInvalidParam));
}

auto ContextInfo::add_property(String name, const AttributeType type) -> Result<void>
{
  auto [iter, did_insert] = mProperties.try_emplace(std::move(name), type);
  return did_insert ? kOK : unexpected(make_error(GenericError::kInvalidParam));
}

auto ContextInfo::update_property(StringView name, Attribute value) -> Result<void>
{
  if (auto* property = find_property(name)) {
    *property = std::move(value);
    return kOK;
  }
  else {
    return unexpected(make_error(GenericError::kInvalidParam));
  }
}

auto ContextInfo::remove_property(StringView name) -> Maybe<Attribute>
{
  if (const auto iter = mProperties.find(name); iter != mProperties.end()) {
    auto removed_property = iter->second;
    mProperties.erase(iter);
    return removed_property;
  }
  else {
    return nothing;
  }
}

auto ContextInfo::rename_property(StringView current_name, String new_name)
    -> Result<void>
{
  if (has_property(new_name)) {
    return unexpected(make_error(GenericError::kInvalidParam));
  }

  if (const auto iter = mProperties.find(current_name); iter != mProperties.end()) {
    auto property_value = iter->second;

    mProperties.erase(iter);
    mProperties[std::move(new_name)] = std::move(property_value);

    return kOK;
  }

  return unexpected(make_error(GenericError::kInvalidParam));
}

auto ContextInfo::attach_component(Component component) -> Result<void>
{
  const auto component_id = component.definition_id();
  auto [iter, did_insert] = mComponents.try_emplace(component_id, std::move(component));
  return did_insert ? kOK : unexpected(make_error(GenericError::kInvalidParam));
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

auto ContextInfo::get_property(StringView name) -> Attribute&
{
  return lookup_in(mProperties, name);
}

auto ContextInfo::get_property(StringView name) const -> const Attribute&
{
  return lookup_in(mProperties, name);
}

auto ContextInfo::get_component(const UUID& component_id) -> Component&
{
  return lookup_in(mComponents, component_id);
}

auto ContextInfo::get_component(const UUID& component_id) const -> const Component&
{
  return lookup_in(mComponents, component_id);
}

auto ContextInfo::find_property(StringView name) -> Attribute*
{
  if (const auto iter = mProperties.find(name); iter != mProperties.end()) {
    return &iter->second;
  }
  else {
    return nullptr;
  }
}

auto ContextInfo::find_property(StringView name) const -> const Attribute*
{
  if (const auto iter = mProperties.find(name); iter != mProperties.end()) {
    return &iter->second;
  }
  else {
    return nullptr;
  }
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

auto ContextInfo::has_property(StringView name) const -> bool
{
  return mProperties.contains(name);
}

auto ContextInfo::has_component(const UUID& component_id) const -> bool
{
  return mComponents.contains(component_id);
}

auto ContextInfo::property_count() const -> usize
{
  return mProperties.size();
}

auto ContextInfo::component_count() const -> usize
{
  return mComponents.size();
}

auto ContextInfo::clone() const -> ContextInfo
{
  ContextInfo other {*this};
  other.mId = UUID::generate();
  return other;
}

}  // namespace tactile
