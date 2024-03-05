/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include "core/context/context_visitor.hpp"
#include "core/element.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

TACTILE_FWD_DECLARE_CLASS(ContextInfo)

/// Interface for all objects that may feature properties and components.
/// Remember to update the context visitor when adding a new context implementation.
class Context : public Element {
 public:
  virtual void accept(ContextVisitor& visitor) const = 0;

  [[nodiscard]] virtual auto get_ctx() -> ContextInfo& = 0;
  [[nodiscard]] virtual auto get_ctx() const -> const ContextInfo& = 0;
};

}  // namespace tactile
