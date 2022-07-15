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

#include <string>  // string

#include "core/attribute.hpp"
#include "core/common/memory.hpp"
#include "core/common/uuid.hpp"
#include "core/comp/component_index.hpp"

namespace tactile::test {

class ComponentBuilder
{
 public:
  ComponentBuilder(Shared<ComponentIndex> index, std::string name);

  auto with_attr(std::string name, Attribute value = std::string {}) -> ComponentBuilder&;

  [[nodiscard]] auto result() -> UUID { return mComponentId; }

 private:
  Shared<ComponentIndex> mIndex;
  UUID                   mComponentId;
};

}  // namespace tactile::test
