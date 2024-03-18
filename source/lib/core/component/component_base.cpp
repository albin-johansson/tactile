// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "component_base.hpp"

#include <utility>  // move

#include <fmt/format.h>

#include "tactile/core/debug/exception.hpp"
#include "tactile/core/debug/generic_error.hpp"
#include "tactile/core/util/lookup.hpp"

namespace tactile {

ComponentBase::ComponentBase(const UUID& type)
  : mTypeId {type}
{}

void ComponentBase::add_attr(String key, AttributeType type)
{
  if (!exists_in(mAttributes, key)) {
    mAttributes[std::move(key)].reset(type);
  }
  else {
    throw Exception {"Attribute key must be unique!"};
  }
}

void ComponentBase::add_attr(String key, Attribute value)
{
  if (!exists_in(mAttributes, key)) {
    mAttributes[std::move(key)] = std::move(value);
  }
  else {
    throw Exception {"Attribute key must be unique!"};
  }
}

void ComponentBase::update_attr(StringView key, Attribute value)
{
  auto& attribute = lookup_in(mAttributes, key);
  attribute = std::move(value);
}

auto ComponentBase::remove_attr(StringView key) -> Result<void>
{
  if (const auto iter = mAttributes.find(key); iter != mAttributes.end()) {
    mAttributes.erase(iter);
    return kOK;
  }
  else {
    return unexpected(make_error(GenericError::kInvalidParam));
  }
}

auto ComponentBase::rename_attr(StringView old_key, String new_key) -> Result<void>
{
  if (exists_in(mAttributes, new_key)) {
    return unexpected(make_error(GenericError::kInvalidParam));
  }

  if (const auto iter = mAttributes.find(old_key); iter != mAttributes.end()) {
    auto value = iter->second;
    mAttributes.erase(iter);
    mAttributes[std::move(new_key)] = std::move(value);
    return kOK;
  }
  else {
    return unexpected(make_error(GenericError::kInvalidParam));
  }
}

auto ComponentBase::duplicate_attr(StringView key) -> String
{
  auto value = lookup_in(mAttributes, key);

  int suffix = 1;
  String new_key;
  do {
    new_key = fmt::format("{} ({})", key, suffix);
    ++suffix;
  } while (exists_in(mAttributes, new_key));

  mAttributes[new_key] = std::move(value);
  return new_key;
}

auto ComponentBase::get_attr(StringView key) const -> const Attribute&
{
  return lookup_in(mAttributes, key);
}

auto ComponentBase::has_attr(StringView key) const -> bool
{
  return exists_in(mAttributes, key);
}

auto ComponentBase::attr_count() const -> usize
{
  return mAttributes.size();
}

auto ComponentBase::empty() const -> bool
{
  return mAttributes.empty();
}

}  // namespace tactile
