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

#include "property_type_combo.hpp"

#include <algorithm>  // find_if
#include <array>      // array
#include <cstring>    // strcmp
#include <utility>    // pair, make_pair

#include <imgui.h>

#include "editor/gui/scoped.hpp"
#include "misc/throw.hpp"
#include "tactile.hpp"

namespace tactile {
namespace {

constexpr std::array gItems{std::make_pair("string", AttributeType::string),
                            std::make_pair("int", AttributeType::integer),
                            std::make_pair("float", AttributeType::floating),
                            std::make_pair("bool", AttributeType::boolean),
                            std::make_pair("color", AttributeType::color),
                            std::make_pair("object", AttributeType::object),
                            std::make_pair("file", AttributeType::file)};

[[nodiscard]] auto GetIndexFromType(const AttributeType type) -> usize
{
  const auto it = std::find_if(
      gItems.begin(),
      gItems.end(),
      [=](const std::pair<c_str, AttributeType>& pair) { return type == pair.second; });

  if (it != gItems.end()) {
    return static_cast<usize>(it - gItems.begin());
  }
  else {
    throw_traced(tactile_error{"Invalid property type!"});
  }
}

void PropertyTypeComboImpl(AttributeType& out, Maybe<AttributeType> previousType)
{
  const auto currentIndex = GetIndexFromType(out);
  auto&& [currentName, currentType] = gItems.at(currentIndex);

  if (scoped::combo combo{"##PropertyTypeComboImpl", currentName}; combo.is_open()) {
    for (auto&& [name, type] : gItems) {
      scoped::disable disable{previousType == type};

      const auto selected = std::strcmp(currentName, name) == 0;
      if (ImGui::Selectable(name, selected)) {
        out = type;
      }

      if (selected) {
        ImGui::SetItemDefaultFocus();
      }
    }
  }
}

}  // namespace

void PropertyTypeCombo(AttributeType& out)
{
  PropertyTypeComboImpl(out, nothing);
}

void PropertyTypeCombo(const AttributeType previous, AttributeType& out)
{
  PropertyTypeComboImpl(out, previous);
}

}  // namespace tactile
