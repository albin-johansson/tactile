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
  ///
  /// \param[in] name
  ///            The name of the attribute.
  /// \param[in] kind
  ///            The kind of attribute to create.
  /// \returns   The new attribute.
  auto create_or_replace(String name, AttributeKind kind = AttributeKind::kInt) -> Attribute&;

  /// Removes an attribute from the set.
  ///
  /// \param[in] name
  ///            The name of the attribute to remove.
  /// \returns   The removed attribute if successful; an empty optional otherwise.
  auto remove(StringView name) -> Option<Attribute>;

  /// Returns a pointer to the attribute with a given name.
  ///
  /// \param[in] name
  ///            The name of the target attribute.
  /// \returns   A pointer to the attribute if found; a null pointer otherwise.
  auto try_get(StringView name) -> Attribute*;

  /// \copydoc try_get
  auto try_get(StringView name) const -> const Attribute*;

  /// Returns the attribute with a given name.
  ///
  /// \param[in] name
  ///            The name of the target attribute.
  /// \returns   The found attribute.
  ///
  /// \throws    std::invalid_argument
  ///            If there's no attribute associated with the name.
  auto get(StringView name) -> Attribute&;

  /// \copydoc get
  auto get(StringView name) const -> const Attribute&;

  /// Returns the number of attributes in the set.
  ///
  /// \returns The attribute count.
  auto size() const -> usize;

  /// Visits each attribute in the set.
  ///
  /// \tparam    T
  ///            The visitor type.
  /// \param[in] visitor
  ///            The visitor to invoke for each attribute.
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
