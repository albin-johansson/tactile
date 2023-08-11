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

#include <unordered_map>  // unordered_map

#include "tactile/core/common/prelude.hpp"
#include "tactile/core/contexts/component.hpp"
#include "tactile/core/core.hpp"

namespace tactile {

class TACTILE_CORE_API ComponentBundle final {
 public:
  using ComponentMap = std::unordered_map<ComponentID, ComponentInstance>;
  using const_iterator = typename ComponentMap::const_iterator;

  void attach(ComponentInstance component);

  auto detach(ComponentID id) -> ComponentInstance;

  [[nodiscard]] auto get(ComponentID id) -> ComponentInstance&;
  [[nodiscard]] auto get(ComponentID id) const -> const ComponentInstance&;

  [[nodiscard]] auto has(ComponentID id) const -> bool;

  [[nodiscard]] auto size() const -> usize;

  [[nodiscard]] auto begin() const noexcept -> const_iterator;

  [[nodiscard]] auto end() const noexcept -> const_iterator;

 private:
  ComponentMap mComponents;
};

}  // namespace tactile
