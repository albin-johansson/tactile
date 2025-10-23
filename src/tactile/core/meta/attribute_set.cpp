// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "tactile/core/meta/attribute_set.hpp"

#include <utility>

#include "tactile/core/container/lookup.hpp"

namespace tactile {

auto AttributeSet::create_or_replace(String name, const AttributeKind kind) -> Attribute&
{
  auto [iter, _] = m_attributes.insert_or_assign(std::move(name), Attribute {});
  iter->second.reset(kind);
  return iter->second;
}

auto AttributeSet::remove(const StringView name) -> Option<Attribute>
{
  return extract_from(m_attributes, name);
}

auto AttributeSet::try_get(const StringView name) -> Attribute*
{
  return try_get_from(m_attributes, name);
}

auto AttributeSet::try_get(const StringView name) const -> const Attribute*
{
  return try_get_from(m_attributes, name);
}

auto AttributeSet::get(const StringView name) -> Attribute&
{
  return get_from(m_attributes, name);
}

auto AttributeSet::get(const StringView name) const -> const Attribute&
{
  return get_from(m_attributes, name);
}

auto AttributeSet::size() const -> usize
{
  return m_attributes.size();
}

}  // namespace tactile
