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
#include <optional>   // optional
#include <utility>    // pair, make_pair

#include <imgui.h>

#include "editor/gui/scoped.hpp"
#include "misc/throw.hpp"
#include "tactile.hpp"

namespace tactile {
namespace {

constexpr std::array _items{std::make_pair("string", AttributeType::String),
                            std::make_pair("int", AttributeType::Int),
                            std::make_pair("float", AttributeType::Float),
                            std::make_pair("bool", AttributeType::Bool),
                            std::make_pair("color", AttributeType::Color),
                            std::make_pair("object", AttributeType::Object),
                            std::make_pair("file", AttributeType::Path)};

[[nodiscard]] auto _index_from_type(const AttributeType type) -> usize
{
  auto iter = std::find_if(_items.begin(), _items.end(), [=](const auto& pair) {
    return type == pair.second;
  });

  if (iter != _items.end()) {
    return static_cast<usize>(iter - _items.begin());
  }
  else {
    panic("Invalid property type!");
  }
}

void _property_type_combo_impl(AttributeType& out,
                               std::optional<AttributeType> previousType)
{
  const auto currentIndex = _index_from_type(out);
  auto&& [currentName, currentType] = _items.at(currentIndex);

  if (scoped::Combo combo{"##_property_type_combo_impl", currentName}; combo.is_open()) {
    for (auto&& [name, type] : _items) {
      scoped::Disable disable{previousType == type};

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

void show_property_type_combo(AttributeType& out)
{
  _property_type_combo_impl(out, std::nullopt);
}

void show_property_type_combo(const AttributeType previous, AttributeType& out)
{
  _property_type_combo_impl(out, previous);
}

}  // namespace tactile
