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
#include <utility>    // make_pair

#include <imgui.h>

#include "core/type/array.hpp"
#include "core/type/maybe.hpp"
#include "core/type/pair.hpp"
#include "core/type/string.hpp"
#include "core/vocabulary.hpp"
#include "core/debug/panic.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "ui/widget/scoped.hpp"

namespace tactile::ui {
namespace {

[[nodiscard]] auto index_from_type(const auto& items, const AttributeType type) -> usize
{
  auto iter = std::find_if(items.begin(), items.end(), [=](const auto& pair) {
    return type == pair.second;
  });

  if (iter != items.end()) {
    return static_cast<usize>(iter - items.begin());
  }
  else {
    throw TactileError {"Invalid property type!"};
  }
}

void property_type_combo_impl(AttributeType& out, Maybe<AttributeType> previous_type)
{
  const auto& lang = get_current_language();

  using TypeArray = Array<Pair<StringView, AttributeType>, 7>;
  const TypeArray items {
      std::make_pair(lang.misc.type_string.c_str(), AttributeType::String),
      std::make_pair(lang.misc.type_int.c_str(), AttributeType::Int),
      std::make_pair(lang.misc.type_float.c_str(), AttributeType::Float),
      std::make_pair(lang.misc.type_bool.c_str(), AttributeType::Bool),
      std::make_pair(lang.misc.type_color.c_str(), AttributeType::Color),
      std::make_pair(lang.misc.type_object.c_str(), AttributeType::Object),
      std::make_pair(lang.misc.type_path.c_str(), AttributeType::Path),
  };

  const auto current_index = index_from_type(items, out);
  auto&& [current_name, current_type] = items.at(current_index);

  if (Combo combo {"##PropertyTypeCombo", current_name.data()}; combo.is_open()) {
    for (auto&& [name, type]: items) {
      Disable disable_if {previous_type == type};

      const auto selected = current_name == name;
      if (Selectable::Property(name.data(), selected)) {
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
  property_type_combo_impl(out, nothing);
}

void show_property_type_combo(const AttributeType previous, AttributeType& out)
{
  property_type_combo_impl(out, previous);
}

}  // namespace tactile::ui
