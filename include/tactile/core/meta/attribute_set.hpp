// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#pragma once

#include <concepts>
#include <utility>

#include "tactile/core/container/option.hpp"
#include "tactile/core/container/string.hpp"
#include "tactile/core/container/tree_map.hpp"
#include "tactile/core/meta/attribute.hpp"

namespace tactile {

/// Represents an ordered set of attributes.
class AttributeSet final
{
 public:
  /// Adds or resets an attribute in the set.
  auto create_or_replace(String name, AttributeKind kind = AttributeKind::kInt)
      -> Attribute&;

  /// Removes an attribute from the set.
  auto remove(StringView name) -> Option<Attribute>;

  /// Returns a pointer to the attribute with a given name, or null if there is none.
  auto try_get(StringView name) -> Attribute*;
  auto try_get(StringView name) const -> const Attribute*;

  /// Returns the attribute with a given name.
  auto get(StringView name) -> Attribute&;
  auto get(StringView name) const -> const Attribute&;

  /// Returns the number of attributes in the set.
  auto size() const -> usize;

  /// Visits each attribute in the set.
  template <std::invocable<const String&, const Attribute&> T>
  void each(const T& visitor) const
  {
    for (const auto& [key, value] : m_attributes) {
      visitor(key, value);
    }
  }

 private:
  TreeMap<String, Attribute> m_attributes {};
};

}  // namespace tactile
