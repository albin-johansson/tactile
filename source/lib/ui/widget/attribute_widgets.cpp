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

#include "attribute_widgets.hpp"

#include <concepts>  // invocable
#include <utility>   // move, to_underlying

#include <glm/gtc/type_ptr.hpp>

#include "common/type/array.hpp"
#include "common/util/filesystem.hpp"
#include "common/util/string_buffer.hpp"
#include "io/file_dialog.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "ui/constants.hpp"
#include "ui/style/colors.hpp"
#include "ui/style/icons.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile::ui {
namespace {

template <std::invocable T>
[[nodiscard]] auto input_file_path(const char* id, StringView text, T&& callback)
    -> Maybe<Path>
{
  const Scope scope {id};

  if (ui_button(TAC_ICON_THREE_DOTS)) {
    if (auto result = callback()) {
      return std::move(*result);
    }
  }

  ImGui::SameLine();

  const auto& lang = get_current_language();

  StringBuffer buffer;
  buffer = text;

  ImGui::InputTextWithHint("##Path",
                           lang.misc.empty.c_str(),
                           buffer.data(),
                           sizeof buffer,
                           ImGuiInputTextFlags_ReadOnly);

  ui_lazy_tooltip("##PathTooltip", lang.misc.type_path.c_str());

  return nothing;
}

}  // namespace

auto ui_int_input(const char* id, int value) -> Maybe<int>
{
  const Scope scope {id};

  ImGui::SetNextItemWidth(-kMinFloat);
  if (ImGui::DragInt("##Int", &value)) {
    return value;
  }

  const auto& lang = get_current_language();
  ui_lazy_tooltip("##IntTooltip", lang.misc.type_int.c_str());

  return nothing;
}

auto ui_int2_input(const char* id, Int2 value) -> Maybe<Int2>
{
  const Scope scope {id};

  ImGui::SetNextItemWidth(-kMinFloat);
  if (ImGui::DragInt2("##Int2", glm::value_ptr(value))) {
    return value;
  }

  const auto& lang = get_current_language();
  ui_lazy_tooltip("##Int2Tooltip", lang.misc.type_int2.c_str());

  return nothing;
}

auto ui_int3_input(const char* id, Int3 value) -> Maybe<Int3>
{
  const Scope scope {id};

  ImGui::SetNextItemWidth(-kMinFloat);
  if (ImGui::DragInt3("##Int3", glm::value_ptr(value))) {
    return value;
  }

  const auto& lang = get_current_language();
  ui_lazy_tooltip("##Int3Tooltip", lang.misc.type_int3.c_str());

  return nothing;
}

auto ui_int4_input(const char* id, Int4 value) -> Maybe<Int4>
{
  const Scope scope {id};

  ImGui::SetNextItemWidth(-kMinFloat);
  if (ImGui::DragInt4("##Int4", glm::value_ptr(value))) {
    return value;
  }

  const auto& lang = get_current_language();
  ui_lazy_tooltip("##Int4Tooltip", lang.misc.type_int4.c_str());

  return nothing;
}

auto ui_float_input(const char* id, float value, const float min, const float max)
    -> Maybe<float>
{
  const Scope scope {id};

  ImGui::SetNextItemWidth(-kMinFloat);

  if (min != 0 || max != 0) {
    if (ImGui::SliderFloat("##Float", &value, min, max)) {
      return value;
    }
  }
  else {
    if (ImGui::DragFloat("##Float", &value)) {
      return value;
    }
  }

  const auto& lang = get_current_language();
  ui_lazy_tooltip("##FloatTooltip", lang.misc.type_float.c_str());

  return nothing;
}

auto ui_float2_input(const char* id, Float2 value, const float min, const float max)
    -> Maybe<Float2>
{
  const Scope scope {id};

  ImGui::SetNextItemWidth(-kMinFloat);

  if (min != 0 || max != 0) {
    if (ImGui::SliderFloat2("##Float2", glm::value_ptr(value), min, max)) {
      return value;
    }
  }
  else {
    if (ImGui::DragFloat2("##Float2", glm::value_ptr(value))) {
      return value;
    }
  }

  const auto& lang = get_current_language();
  ui_lazy_tooltip("##Float2Tooltip", lang.misc.type_float2.c_str());

  return nothing;
}

auto ui_float3_input(const char* id, Float3 value, const float min, const float max)
    -> Maybe<Float3>
{
  const Scope scope {id};

  ImGui::SetNextItemWidth(-kMinFloat);

  if (min != 0 || max != 0) {
    if (ImGui::SliderFloat3("##Float3", glm::value_ptr(value), min, max)) {
      return value;
    }
  }
  else {
    if (ImGui::DragFloat3("##Float3", glm::value_ptr(value))) {
      return value;
    }
  }

  const auto& lang = get_current_language();
  ui_lazy_tooltip("##Float3Tooltip", lang.misc.type_float3.c_str());

  return nothing;
}

auto ui_float4_input(const char* id, Float4 value, const float min, const float max)
    -> Maybe<Float4>
{
  const Scope scope {id};

  ImGui::SetNextItemWidth(-kMinFloat);

  if (min != 0 || max != 0) {
    if (ImGui::SliderFloat4("##Float4", glm::value_ptr(value), min, max)) {
      return value;
    }
  }
  else {
    if (ImGui::DragFloat4("##Float4", glm::value_ptr(value))) {
      return value;
    }
  }

  const auto& lang = get_current_language();
  ui_lazy_tooltip("##Float4Tooltip", lang.misc.type_float4.c_str());

  return nothing;
}

auto ui_string_input_with_hint(const char* id,
                               const char* hint,
                               const String& value,
                               const char* label,
                               const ImGuiInputTextFlags flags,
                               const ImGuiInputTextCallback filter) -> Maybe<String>
{
  const Scope scope {id};

  StringBuffer buffer;
  buffer = value;

  if (label) {
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(label);

    ImGui::SameLine();
  }
  else {
    ImGui::SetNextItemWidth(-kMinFloat);
  }

  if (ImGui::InputTextWithHint("##String",
                               hint ? hint : "",
                               buffer.data(),
                               sizeof buffer,
                               flags,
                               filter)) {
    return buffer.as_string();
  }

  const auto& lang = get_current_language();
  ui_lazy_tooltip("##StringTooltip", lang.misc.type_string.c_str());

  return nothing;
}

auto ui_string_input(const char* id,
                     const String& value,
                     const char* label,
                     const ImGuiInputTextFlags flags,
                     const ImGuiInputTextCallback filter) -> Maybe<String>
{
  return ui_string_input_with_hint(id, nullptr, value, label, flags, filter);
}

auto ui_bool_input(const char* id, bool value) -> Maybe<bool>
{
  const Scope scope {id};

  if (ImGui::Checkbox("##Bool", &value)) {
    return value;
  }

  const auto& lang = get_current_language();
  ui_lazy_tooltip("##BoolTooltip", lang.misc.type_bool.c_str());

  return nothing;
}

auto ui_object_input(const char* id, ObjectRef value) -> Maybe<ObjectRef>
{
  const Scope scope {id};

  // TODO
  ImGui::Text("%i", value);

  const auto& lang = get_current_language();
  ui_lazy_tooltip("##ObjectTooltip", lang.misc.type_object.c_str());

  return nothing;
}

auto ui_color_input(const char* id, const Color value) -> Maybe<Color>
{
  constexpr auto flags = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel |
                         ImGuiColorEditFlags_AlphaBar;

  const Scope scope {id};

  auto rgba = value.as_float_array();
  if (ImGui::ColorEdit4("##Color", rgba.data(), flags)) {
    return Color::from_norm(rgba.at(0), rgba.at(1), rgba.at(2), rgba.at(3));
  }

  return nothing;
}

auto ui_file_path_input(const char* id, const Path& value) -> Maybe<Path>
{
  return input_file_path(id, value.filename().string(), []() -> Maybe<Path> {
    auto dialog = io::FileDialog::open_file();
    return dialog.is_okay() ? dialog.path() : Maybe<Path> {};
  });
}

auto ui_directory_path_input(const char* id, const Path& value) -> Maybe<Path>
{
  return input_file_path(id,
                         use_short_home_prefix(value).value_or(value.string()),
                         []() -> Maybe<Path> {
                           auto dialog = io::FileDialog::open_folder();
                           return dialog.is_okay() ? dialog.path() : Maybe<Path> {};
                         });
}

auto ui_attribute_input(const char* id, const Attribute& value) -> Maybe<Attribute>
{
  switch (value.get_type()) {
    case AttributeType::String: {
      const auto& lang = get_current_language();
      if (auto updated =
              ui_string_input_with_hint(id, lang.misc.empty.c_str(), value.as_string())) {
        return std::move(updated);
      }
      break;
    }
    case AttributeType::Int: {
      if (const auto updated = ui_int_input(id, value.as_int())) {
        return updated;
      }
      break;
    }
    case AttributeType::Int2: {
      if (const auto updated = ui_int2_input(id, value.as_int2())) {
        return updated;
      }
      break;
    }
    case AttributeType::Int3: {
      if (const auto updated = ui_int3_input(id, value.as_int3())) {
        return updated;
      }
      break;
    }
    case AttributeType::Int4: {
      if (const auto updated = ui_int4_input(id, value.as_int4())) {
        return updated;
      }
      break;
    }
    case AttributeType::Float: {
      if (const auto updated = ui_float_input(id, value.as_float())) {
        return updated;
      }
      break;
    }
    case AttributeType::Float2: {
      if (const auto updated = ui_float2_input(id, value.as_float2())) {
        return updated;
      }
      break;
    }
    case AttributeType::Float3: {
      if (const auto updated = ui_float3_input(id, value.as_float3())) {
        return updated;
      }
      break;
    }
    case AttributeType::Float4: {
      if (const auto updated = ui_float4_input(id, value.as_float4())) {
        return updated;
      }
      break;
    }
    case AttributeType::Bool: {
      if (const auto updated = ui_bool_input(id, value.as_bool())) {
        return updated;
      }
      break;
    }
    case AttributeType::Path: {
      if (auto updated = ui_file_path_input(id, value.as_path())) {
        return std::move(updated);
      }
      break;
    }
    case AttributeType::Color: {
      if (const auto updated = ui_color_input(id, value.as_color())) {
        return updated;
      }
      break;
    }
    case AttributeType::Object: {
      if (const auto updated = ui_object_input(id, value.as_object())) {
        return updated;
      }
      break;
    }
  }

  return nothing;
}

auto ui_attribute_type_combo(const AttributeType current_type,
                             const Maybe<AttributeType> excluded_type)
    -> Maybe<AttributeType>
{
  const auto& lang = get_current_language();

  Array<StringView, 13> type_names;
  type_names[std::to_underlying(AttributeType::String)] = lang.misc.type_string.c_str();
  type_names[std::to_underlying(AttributeType::Int)] = lang.misc.type_int.c_str();
  type_names[std::to_underlying(AttributeType::Int2)] = lang.misc.type_int2.c_str();
  type_names[std::to_underlying(AttributeType::Int3)] = lang.misc.type_int3.c_str();
  type_names[std::to_underlying(AttributeType::Int4)] = lang.misc.type_int4.c_str();
  type_names[std::to_underlying(AttributeType::Float)] = lang.misc.type_float.c_str();
  type_names[std::to_underlying(AttributeType::Float2)] = lang.misc.type_float2.c_str();
  type_names[std::to_underlying(AttributeType::Float3)] = lang.misc.type_float3.c_str();
  type_names[std::to_underlying(AttributeType::Float4)] = lang.misc.type_float4.c_str();
  type_names[std::to_underlying(AttributeType::Bool)] = lang.misc.type_bool.c_str();
  type_names[std::to_underlying(AttributeType::Color)] = lang.misc.type_color.c_str();
  type_names[std::to_underlying(AttributeType::Object)] = lang.misc.type_object.c_str();
  type_names[std::to_underlying(AttributeType::Path)] = lang.misc.type_path.c_str();

  const auto all_types = {AttributeType::String,
                          AttributeType::Int,
                          AttributeType::Int2,
                          AttributeType::Int3,
                          AttributeType::Int4,
                          AttributeType::Float,
                          AttributeType::Float2,
                          AttributeType::Float3,
                          AttributeType::Float4,
                          AttributeType::Bool,
                          AttributeType::Color,
                          AttributeType::Object,
                          AttributeType::Path};

  const auto current_type_index = static_cast<usize>(std::to_underlying(current_type));
  const auto& current_type_name = type_names.at(current_type_index);

  Maybe<AttributeType> result;

  if (const Combo combo {"##AttributeTypeCombo", current_type_name.data()};
      combo.is_open()) {
    for (const auto type: all_types) {
      const Disable disable_if_excluded {type == excluded_type};

      const auto type_index = static_cast<usize>(std::to_underlying(type));
      const auto& type_name = type_names.at(type_index);

      const auto selected = current_type_name == type_name;
      if (Selectable::Property(type_name.data(), selected)) {
        result = type;
      }

      if (selected) {
        ImGui::SetItemDefaultFocus();
      }
    }
  }

  return result;
}

}  // namespace tactile::ui
