// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "core/component/component.hpp"
#include "core/component/component_base.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/core/util/uuid.hpp"

namespace tactile {

/// Represents the structure of a component type, providing attributes and default values.
class ComponentDefinition final : public ComponentBase {
 public:
  explicit ComponentDefinition(const UUID& id = UUID::generate());

  /// Sets the unique name of the component definition.
  void set_name(String name);

  /// Returns the (unique) name of the component type.
  [[nodiscard]] auto get_name() const -> const String&;

  [[nodiscard]] auto get_uuid() const -> const UUID&;

  /// Creates a new component using the component definition as a template.
  [[nodiscard]] auto instantiate() const -> Component;

 private:
  String mName;
};

}  // namespace tactile
