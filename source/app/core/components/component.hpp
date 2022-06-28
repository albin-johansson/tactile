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

#include <string>       // string
#include <string_view>  // string_view

#include "core/attribute.hpp"
#include "core/common/associative.hpp"
#include "core/common/uuid.hpp"

namespace tactile::core {

class Component final
{
 public:
  using AttributeMap = TreeMap<std::string, Attribute>;

  Component(const UUID& definitionId, AttributeMap attributes);

  void add_attr(std::string key, Attribute value);

  void remove_attr(std::string_view key);

  void update_attr(std::string_view key, Attribute value);

  [[nodiscard]] auto get_attr(std::string_view key) const -> const Attribute&;

  [[nodiscard]] auto definition_id() const -> const UUID& { return mDefinitionId; }

  [[nodiscard]] auto begin() const noexcept { return mAttributes.begin(); }
  [[nodiscard]] auto end() const noexcept { return mAttributes.end(); }

 private:
  UUID         mDefinitionId;
  AttributeMap mAttributes;
};

}  // namespace tactile::core
