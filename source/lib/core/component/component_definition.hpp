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

#pragma once

#include "core/component/component.hpp"
#include "core/component/component_base.hpp"
#include "core/element.hpp"
#include "common/type/string.hpp"
#include "common/type/uuid.hpp"
#include "core/vocabulary.hpp"

namespace tactile {

/// Represents the structure of a component type, providing attributes and default values.
class ComponentDefinition final : public ComponentBase, public Element {
 public:
  explicit ComponentDefinition(const UUID& id = make_uuid());

  /// Sets the unique name of the component definition.
  void set_name(String name);

  /// Returns the (unique) name of the component type.
  [[nodiscard]] auto name() const -> const String&;

  [[nodiscard]] auto get_uuid() const -> const UUID& override;

  /// Creates a new component using the component definition as a template.
  [[nodiscard]] auto instantiate() const -> Component;

 private:
  String mName;
};

}  // namespace tactile
