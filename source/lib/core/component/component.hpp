// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "core/component/component_base.hpp"
#include "tactile/core/util/uuid.hpp"

namespace tactile {

/// Represents an instance of a component definition.
class Component final : public ComponentBase {
 public:
  /// Creates a component.
  ///
  /// \param definition_id the ID of a component definition.
  /// \param attributes the default attribute values.
  Component(const UUID& definition_id, AttributeMap attributes);

  /// Returns the ID of the associated component definition type.
  [[nodiscard]] auto definition_id() const -> const UUID&;
};

}  // namespace tactile
