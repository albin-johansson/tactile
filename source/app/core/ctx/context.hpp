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

#include "core/ctx/context_visitor.hpp"
#include "core/element.hpp"

namespace tactile {

class ContextInfo;

/// Interface for all objects that may feature properties and components.
/// Remember to update IContextVisitor when adding a new context implementation.
class IContext : public IElement {
 public:
  virtual void accept(IContextVisitor& visitor) const = 0;

  [[nodiscard]] virtual auto ctx() -> ContextInfo& = 0;
  [[nodiscard]] virtual auto ctx() const -> const ContextInfo& = 0;
};

}  // namespace tactile
