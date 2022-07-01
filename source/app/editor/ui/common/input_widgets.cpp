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

#include "input_widgets.hpp"

#include <array>    // array
#include <utility>  // move

#include "colors.hpp"
#include "core/utils/buffers.hpp"
#include "editor/constants.hpp"
#include "editor/ui/common/button.hpp"
#include "editor/ui/icons.hpp"
#include "editor/ui/scoped.hpp"
#include "io/file_dialog.hpp"

namespace tactile::ui {

auto input_attribute(const char* id, const Attribute& value) -> Maybe<Attribute>
{
  switch (value.type()) {
    case AttributeType::String: {
      if (auto updated = input_string_with_hint(id, "Empty", value.as_string())) {
        return std::move(updated);
      }
      break;
    }
    case AttributeType::Int: {
      if (const auto updated = input_int(id, value.as_int())) {
        return updated;
      }
      break;
    }
    case AttributeType::Float: {
      if (const auto updated = input_float(id, value.as_float())) {
        return updated;
      }
      break;
    }
    case AttributeType::Bool: {
      if (const auto updated = input_bool(id, value.as_bool())) {
        return updated;
      }
      break;
    }
    case AttributeType::Path: {
      if (auto updated = input_path(id, value.as_path())) {
        return std::move(updated);
      }
      break;
    }
    case AttributeType::Color: {
      if (const auto updated = input_color(id, value.as_color())) {
        return updated;
      }
      break;
    }
    case AttributeType::Object: {
      if (const auto updated = input_object(id, value.as_object())) {
        return updated;
      }
      break;
    }
  }

  return nothing;
}

auto input_int(const char* id, int value) -> Maybe<int>
{
  const Scope scope{id};

  ImGui::SetNextItemWidth(-min_float);
  if (ImGui::DragInt("##input_int", &value)) {
    return value;
  }

  if (ImGui::IsItemHovered()) {
    ImGui::SetTooltip("[int]");
  }

  return nothing;
}

auto input_float(const char* id, float value, const float min, const float max)
    -> Maybe<float>
{
  const Scope scope{id};

  ImGui::SetNextItemWidth(-min_float);

  if (min != 0 || max != 0) {
    if (ImGui::SliderFloat("##input_float", &value, min, max)) {
      return value;
    }
  }
  else {
    if (ImGui::DragFloat("##input_float", &value)) {
      return value;
    }
  }

  if (ImGui::IsItemHovered()) {
    ImGui::SetTooltip("[float]");
  }

  return nothing;
}

auto input_string_with_hint(const char*                  id,
                            const char*                  hint,
                            const std::string&           value,
                            const char*                  label,
                            const ImGuiInputTextFlags    flags,
                            const ImGuiInputTextCallback filter) -> Maybe<std::string>
{
  const Scope scope{id};

  std::array<char, 100> buffer;  // NOLINT safely uninitialized
  copy_string_into_buffer(buffer, value);

  if (label) {
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(label);

    ImGui::SameLine();
  }
  else {
    ImGui::SetNextItemWidth(-min_float);
  }

  if (ImGui::InputTextWithHint("##input_string_with_hint",
                               hint ? hint : "",
                               buffer.data(),
                               sizeof buffer,
                               flags,
                               filter)) {
    return create_string_from_buffer(buffer);
  }

  if (ImGui::IsItemHovered()) {
    ImGui::SetTooltip("[string]");
  }

  return nothing;
}

auto input_string(const char*                  id,
                  const std::string&           value,
                  const char*                  label,
                  const ImGuiInputTextFlags    flags,
                  const ImGuiInputTextCallback filter) -> Maybe<std::string>
{
  return input_string_with_hint(id, nullptr, value, label, flags, filter);
}

auto input_bool(const char* id, bool value) -> Maybe<bool>
{
  const Scope scope{id};

  if (ImGui::Checkbox("##input_bool", &value)) {
    return value;
  }

  if (ImGui::IsItemHovered()) {
    ImGui::SetTooltip("[bool]");
  }

  return nothing;
}

auto input_object(const char* id, object_t value) -> Maybe<object_t>
{
  const Scope scope{id};

  // TODO
  ImGui::Text("%i", value);

  if (ImGui::IsItemHovered()) {
    ImGui::SetTooltip("[object]");
  }

  return nothing;
}

auto input_color(const char* id, const cen::color value) -> Maybe<cen::color>
{
  constexpr auto flags = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel |
                         ImGuiColorEditFlags_AlphaBar;

  const Scope scope{id};

  auto arr = color_to_array(value);
  if (ImGui::ColorEdit4("##input_color", arr.data(), flags)) {
    return cen::color::from_norm(arr.at(0), arr.at(1), arr.at(2), arr.at(3));
  }

  if (ImGui::IsItemHovered()) {
    ImGui::SetTooltip("[color]");
  }

  return nothing;
}

auto input_path(const char* id, const std::filesystem::path& value)
    -> Maybe<std::filesystem::path>
{
  const Scope scope{id};

  if (button(TAC_ICON_THREE_DOTS)) {
    auto dialog = io::FileDialog::open_file();
    if (dialog.is_okay()) {
      return dialog.path();
    }
  }

  ImGui::SameLine();

  auto str = value.filename().string();

  ImGui::SetNextItemWidth(-min_float);
  ImGui::InputTextWithHint("##input_path",
                           "N/A",
                           str.data(),
                           str.capacity(),
                           ImGuiInputTextFlags_ReadOnly);

  if (ImGui::IsItemHovered()) {
    ImGui::SetTooltip("[path]");
  }

  return nothing;
}

}  // namespace tactile::ui
