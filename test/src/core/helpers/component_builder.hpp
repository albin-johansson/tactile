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

#include "core/attribute.hpp"
#include "core/component/component_index.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/core/util/uuid.hpp"

namespace tactile::test {

class ComponentBuilder {
 public:
  ComponentBuilder(Shared<ComponentIndex> index, String name);

  auto with_attr(String name, Attribute value = String {}) -> ComponentBuilder&;

  [[nodiscard]] auto result() -> UUID { return mComponentId; }

 private:
  Shared<ComponentIndex> mIndex;
  UUID mComponentId;
};

}  // namespace tactile::test
