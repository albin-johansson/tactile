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

#include "attribute_widgets.hpp"

#include <concepts>  // invocable
#include <utility>   // move, to_underlying

#include <glm/gtc/type_ptr.hpp>

#include "common/util/string_buffer.hpp"
#include "io/file_dialog.hpp"
#include "tactile/core/containers/array.hpp"
#include "tactile/core/io/filesystem.hpp"
#include "ui/constants.hpp"
#include "ui/style/colors.hpp"
#include "ui/style/icons.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile::ui {
namespace {

template <std::invocable T>
[[nodiscard]] auto _input_file_path(const Strings& strings,
                                    const char* id,
                                    StringView text,
                                    T&& callback) -> Maybe<Path>
{
  const Scope scope {id};

  if (push_button(TAC_ICON_THREE_DOTS)) {
    if (auto result = callback()) {
      return std::move(*result);
    }
  }

  ImGui::SameLine();

  StringBuffer buffer;
  buffer = text;

  ImGui::InputTextWithHint("##Path",
                           strings.misc.empty.c_str(),
                           buffer.data(),
                           sizeof buffer,
                           ImGuiInputTextFlags_ReadOnly);

  push_lazy_tooltip("##PathTooltip", strings.misc.type_path.c_str());

  return kNone;
}

}  // namespace

auto push_int_input(const Strings& strings, const char* id, int value) -> Maybe<int>
{
  const Scope scope {id};

  ImGui::SetNextItemWidth(-kMinFloat);
  if (ImGui::DragInt("##Int", &value)) {
    return value;
  }

  push_lazy_tooltip("##IntTooltip", strings.misc.type_int.c_str());

  return kNone;
}

auto push_int2_input(const Strings& strings, const char* id, Int2 value) -> Maybe<Int2>
{
  const Scope scope {id};

  ImGui::SetNextItemWidth(-kMinFloat);
  if (ImGui::DragInt2("##Int2", glm::value_ptr(value))) {
    return value;
  }

  push_lazy_tooltip("##Int2Tooltip", strings.misc.type_int2.c_str());

  return kNone;
}

auto push_int3_input(const Strings& strings, const char* id, Int3 value) -> Maybe<Int3>
{
  const Scope scope {id};

  ImGui::SetNextItemWidth(-kMinFloat);
  if (ImGui::DragInt3("##Int3", glm::value_ptr(value))) {
    return value;
  }

  push_lazy_tooltip("##Int3Tooltip", strings.misc.type_int3.c_str());

  return kNone;
}

auto push_int4_input(const Strings& strings, const char* id, Int4 value) -> Maybe<Int4>
{
  const Scope scope {id};

  ImGui::SetNextItemWidth(-kMinFloat);
  if (ImGui::DragInt4("##Int4", glm::value_ptr(value))) {
    return value;
  }

  push_lazy_tooltip("##Int4Tooltip", strings.misc.type_int4.c_str());

  return kNone;
}

auto push_float_input(const Strings& strings,
                      const char* id,
                      float value,
                      const float min,
                      const float max) -> Maybe<float>
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

  push_lazy_tooltip("##FloatTooltip", strings.misc.type_float.c_str());

  return kNone;
}

auto push_float2_input(const Strings& strings,
                       const char* id,
                       Float2 value,
                       const float min,
                       const float max) -> Maybe<Float2>
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

  push_lazy_tooltip("##Float2Tooltip", strings.misc.type_float2.c_str());

  return kNone;
}

auto push_float3_input(const Strings& strings,
                       const char* id,
                       Float3 value,
                       const float min,
                       const float max) -> Maybe<Float3>
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

  push_lazy_tooltip("##Float3Tooltip", strings.misc.type_float3.c_str());

  return kNone;
}

auto push_float4_input(const Strings& strings,
                       const char* id,
                       Float4 value,
                       const float min,
                       const float max) -> Maybe<Float4>
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

  push_lazy_tooltip("##Float4Tooltip", strings.misc.type_float4.c_str());

  return kNone;
}

auto push_string_input_with_hint(const Strings& strings,
                                 const char* id,
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

  push_lazy_tooltip("##StringTooltip", strings.misc.type_string.c_str());

  return kNone;
}

auto push_string_input(const Strings& strings,
                       const char* id,
                       const String& value,
                       const char* label,
                       const ImGuiInputTextFlags flags,
                       const ImGuiInputTextCallback filter) -> Maybe<String>
{
  return push_string_input_with_hint(strings, id, nullptr, value, label, flags, filter);
}

auto push_bool_input(const Strings& strings, const char* id, bool value) -> Maybe<bool>
{
  const Scope scope {id};

  if (ImGui::Checkbox("##Bool", &value)) {
    return value;
  }

  push_lazy_tooltip("##BoolTooltip", strings.misc.type_bool.c_str());

  return kNone;
}

auto push_object_input(const Strings& strings, const char* id, ObjectRef value)
    -> Maybe<ObjectRef>
{
  const Scope scope {id};

  // TODO
  ImGui::Text("%i", value);

  push_lazy_tooltip("##ObjectTooltip", strings.misc.type_object.c_str());

  return kNone;
}

auto push_color_input(const char* id, const UColor value) -> Maybe<UColor>
{
  constexpr auto flags = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel |
                         ImGuiColorEditFlags_AlphaBar;

  const Scope scope {id};

  auto rgba = normalize(value);
  if (ImGui::ColorEdit4("##Color", &rgba.red, flags)) {
    return unnormalize(rgba);
  }

  return kNone;
}

auto push_file_path_input(const Strings& strings, const char* id, const Path& value)
    -> Maybe<Path>
{
  return _input_file_path(strings, id, value.filename().string(), []() -> Maybe<Path> {
    auto dialog = FileDialog::open_file();
    return dialog.is_okay() ? dialog.path() : Maybe<Path> {};
  });
}

auto push_directory_path_input(const Strings& strings, const char* id, const Path& value)
    -> Maybe<Path>
{
  return _input_file_path(strings,
                          id,
                          use_short_home_prefix(value).value_or(value.string()),
                          []() -> Maybe<Path> {
                            auto dialog = FileDialog::open_folder();
                            return dialog.is_okay() ? dialog.path() : Maybe<Path> {};
                          });
}

auto push_attribute_input(const Strings& strings, const char* id, const Property& value)
    -> Maybe<Property>
{
  switch (value.get_type()) {
    case PropertyType::Str: {
      if (auto updated = push_string_input_with_hint(strings,
                                                     id,
                                                     strings.misc.empty.c_str(),
                                                     value.as_string())) {
        return updated;
      }
      break;
    }
    case PropertyType::Int: {
      if (const auto updated = push_int_input(strings, id, value.as_int())) {
        return updated;
      }
      break;
    }
    case PropertyType::Int2: {
      if (const auto updated = push_int2_input(strings, id, value.as_int2())) {
        return updated;
      }
      break;
    }
    case PropertyType::Int3: {
      if (const auto updated = push_int3_input(strings, id, value.as_int3())) {
        return updated;
      }
      break;
    }
    case PropertyType::Int4: {
      if (const auto updated = push_int4_input(strings, id, value.as_int4())) {
        return updated;
      }
      break;
    }
    case PropertyType::Float: {
      if (const auto updated = push_float_input(strings, id, value.as_float())) {
        return updated;
      }
      break;
    }
    case PropertyType::Float2: {
      if (const auto updated = push_float2_input(strings, id, value.as_float2())) {
        return updated;
      }
      break;
    }
    case PropertyType::Float3: {
      if (const auto updated = push_float3_input(strings, id, value.as_float3())) {
        return updated;
      }
      break;
    }
    case PropertyType::Float4: {
      if (const auto updated = push_float4_input(strings, id, value.as_float4())) {
        return updated;
      }
      break;
    }
    case PropertyType::Bool: {
      if (const auto updated = push_bool_input(strings, id, value.as_bool())) {
        return updated;
      }
      break;
    }
    case PropertyType::Path: {
      if (auto updated = push_file_path_input(strings, id, value.as_path())) {
        return updated;
      }
      break;
    }
    case PropertyType::Color: {
      if (const auto updated = push_color_input(id, value.as_color())) {
        return updated;
      }
      break;
    }
    case PropertyType::Object: {
      if (const auto updated = push_object_input(strings, id, value.as_object())) {
        return updated;
      }
      break;
    }
  }

  return kNone;
}

auto push_attribute_type_combo(const Strings& strings,
                               const PropertyType current_type,
                               const Maybe<PropertyType> excluded_type)
    -> Maybe<PropertyType>
{
  Array<StringView, 13> type_names;
  type_names[std::to_underlying(PropertyType::Str)] = strings.misc.type_string.c_str();
  type_names[std::to_underlying(PropertyType::Int)] = strings.misc.type_int.c_str();
  type_names[std::to_underlying(PropertyType::Int2)] = strings.misc.type_int2.c_str();
  type_names[std::to_underlying(PropertyType::Int3)] = strings.misc.type_int3.c_str();
  type_names[std::to_underlying(PropertyType::Int4)] = strings.misc.type_int4.c_str();
  type_names[std::to_underlying(PropertyType::Float)] = strings.misc.type_float.c_str();
  type_names[std::to_underlying(PropertyType::Float2)] =
      strings.misc.type_float2.c_str();
  type_names[std::to_underlying(PropertyType::Float3)] =
      strings.misc.type_float3.c_str();
  type_names[std::to_underlying(PropertyType::Float4)] =
      strings.misc.type_float4.c_str();
  type_names[std::to_underlying(PropertyType::Bool)] = strings.misc.type_bool.c_str();
  type_names[std::to_underlying(PropertyType::Color)] = strings.misc.type_color.c_str();
  type_names[std::to_underlying(PropertyType::Object)] =
      strings.misc.type_object.c_str();
  type_names[std::to_underlying(PropertyType::Path)] = strings.misc.type_path.c_str();

  const auto all_types = {PropertyType::Str,
                          PropertyType::Int,
                          PropertyType::Int2,
                          PropertyType::Int3,
                          PropertyType::Int4,
                          PropertyType::Float,
                          PropertyType::Float2,
                          PropertyType::Float3,
                          PropertyType::Float4,
                          PropertyType::Bool,
                          PropertyType::Color,
                          PropertyType::Object,
                          PropertyType::Path};

  const auto current_type_index = static_cast<usize>(std::to_underlying(current_type));
  const auto& current_type_name = type_names.at(current_type_index);

  Maybe<PropertyType> result;

  if (const Combo combo {"##AttributeTypeCombo", current_type_name.data()};
      combo.is_open()) {
    for (const auto type: all_types) {
      const Disable disable_if_excluded {type == excluded_type};

      const auto type_index = static_cast<usize>(std::to_underlying(type));
      const auto& type_name = type_names.at(type_index);

      const auto selected = current_type_name == type_name;
      if (Selectable::property(type_name.data(), selected)) {
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
