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

#include "core/common/uuid.hpp"
#include "core/component_bundle.hpp"
#include "core/property_bundle.hpp"

namespace tactile::core {

class ContextDelegate final
{
 public:
  [[nodiscard]] auto get_props() -> PropertyBundle&;
  [[nodiscard]] auto get_props() const -> const PropertyBundle&;

  [[nodiscard]] auto get_comps() -> ComponentBundle&;
  [[nodiscard]] auto get_comps() const -> const ComponentBundle&;

  [[nodiscard]] auto get_uuid() const -> const UUID&;

  [[nodiscard]] auto clone() const -> ContextDelegate;

 private:
  UUID mId{make_uuid()};
  PropertyBundle mProps;
  ComponentBundle mComps;
};

}  // namespace tactile::core