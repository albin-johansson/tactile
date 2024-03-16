// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/expected.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/base/container/string_map.hpp"
#include "tactile/base/int.hpp"
#include "tactile/core/meta/attribute.hpp"
#include "tactile/core/util/uuid.hpp"

namespace tactile {

/// Provides the common API for component definitions and their instances.
class ComponentBase {
 public:
  using AttributeMap = StringMap<Attribute>;

  explicit ComponentBase(const UUID& type);

  /// Creates a new attribute.
  ///
  /// \param key the key to associate with the attribute.
  /// \param type the initial type of the attribute.
  void add_attr(String key, AttributeType type = AttributeType::kStr);

  /// Creates a new attribute.
  ///
  /// \param key the key to associate with the attribute.
  /// \param value the initial value of the attribute.
  void add_attr(String key, Attribute value);

  /// Updates the value of an existing attribute.
  ///
  /// \details
  /// The new value does not have to be of the same type as the old value.
  ///
  /// \param key the key associated with the attribute.
  /// \param value the new attribute value.
  void update_attr(StringView key, Attribute value);

  /// Removes an existing attribute.
  ///
  /// \param key the key associated with the attribute.
  /// \return success if an attribute was removed; failure otherwise.
  auto remove_attr(StringView key) -> Result<void>;

  /// Changes the name (key) of an existing attribute.
  ///
  /// \param old_key the current attribute key.
  /// \param new_key the new attribute key.
  /// \return success if an attribute was renamed; failure otherwise.
  auto rename_attr(StringView old_key, String new_key) -> Result<void>;

  /// Duplicates an existing attribute.
  ///
  /// \param key the name of the attribute to duplicate.
  /// \return the key associated with the new attribute.
  auto duplicate_attr(StringView key) -> String;

  /// Returns the value of an attribute for a specific key.
  [[nodiscard]] auto get_attr(StringView key) const -> const Attribute&;

  /// Indicates whether there is an attribute for a specific key.
  [[nodiscard]] auto has_attr(StringView key) const -> bool;

  /// Returns the amount of attributes in the component.
  [[nodiscard]] auto attr_count() const -> usize;

  /// Indicates whether the component has no attributes.
  [[nodiscard]] auto empty() const -> bool;

  [[nodiscard]] auto begin() const noexcept { return mAttributes.begin(); }
  [[nodiscard]] auto end() const noexcept { return mAttributes.end(); }

 protected:
  UUID mTypeId;
  AttributeMap mAttributes;
};

}  // namespace tactile
