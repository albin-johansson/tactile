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

#include <boost/uuid/uuid_hash.hpp>

#include "core/common/associative.hpp"
#include "core/common/ints.hpp"
#include "core/common/uuid.hpp"
#include "core/components/component.hpp"

namespace tactile::core {

/// Manages a set of components attached to a context.
class ComponentBundle final
{
 public:
  // TODO

  void add(Component component);

  auto erase(const UUID& componentId) -> Component;

  [[nodiscard]] auto at(const UUID& componentId) -> Component&;
  [[nodiscard]] auto at(const UUID& componentId) const -> const Component&;

  [[nodiscard]] auto size() const -> usize;

  [[nodiscard]] auto empty() const -> bool;

  [[nodiscard]] auto begin() const noexcept { return mComps.begin(); }
  [[nodiscard]] auto end() const noexcept { return mComps.end(); }

 private:
  HashMap<UUID, Component> mComps;
};

}  // namespace tactile::core
