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

#include "core/comp/component_base.hpp"
#include "core/uuid.hpp"
#include "core/vocabulary.hpp"

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
