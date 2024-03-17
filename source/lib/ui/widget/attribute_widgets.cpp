// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "attribute_widgets.hpp"

#include <concepts>  // invocable
#include <utility>   // move, to_underlying

#include "common/util/string_buffer.hpp"
#include "io/file_dialog.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "tactile/base/container/array.hpp"
#include "tactile/core/platform/filesystem.hpp"
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
      return result;
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
  if (ImGui::DragInt2("##Int2", value.data())) {
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
  if (ImGui::DragInt3("##Int3", value.data())) {
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
  if (ImGui::DragInt4("##Int4", value.data())) {
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
    if (ImGui::SliderFloat2("##Float2", value.data(), min, max)) {
      return value;
    }
  }
  else {
    if (ImGui::DragFloat2("##Float2", value.data())) {
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
    if (ImGui::SliderFloat3("##Float3", value.data(), min, max)) {
      return value;
    }
  }
  else {
    if (ImGui::DragFloat3("##Float3", value.data())) {
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
    if (ImGui::SliderFloat4("##Float4", value.data(), min, max)) {
      return value;
    }
  }
  else {
    if (ImGui::DragFloat4("##Float4", value.data())) {
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

  auto rgba = value.normalized();
  if (ImGui::ColorEdit4("##Color", rgba.data(), flags)) {
    return Color::from(rgba);
  }

  return nothing;
}

auto ui_file_path_input(const char* id, const Path& value) -> Maybe<Path>
{
  return input_file_path(id, value.filename().string(), []() -> Maybe<Path> {
    auto dialog = FileDialog::open_file();
    return dialog.is_okay() ? dialog.path() : Maybe<Path> {};
  });
}

auto ui_directory_path_input(const char* id, const Path& value) -> Maybe<Path>
{
  static const auto home_dir = get_user_home_directory().value();
  return input_file_path(
      id,
      strip_home_directory_prefix(value, home_dir).value_or(value.string()),
      []() -> Maybe<Path> {
        auto dialog = FileDialog::open_folder();
        return dialog.is_okay() ? dialog.path() : Maybe<Path> {};
      });
}

auto ui_attribute_input(const char* id, const Attribute& value) -> Maybe<Attribute>
{
  switch (value.get_type()) {
    case AttributeType::kStr: {
      const auto& lang = get_current_language();
      if (auto updated =
              ui_string_input_with_hint(id, lang.misc.empty.c_str(), value.as_string())) {
        return std::move(updated);
      }
      break;
    }
    case AttributeType::kInt: {
      if (const auto updated = ui_int_input(id, value.as_int())) {
        return updated;
      }
      break;
    }
    case AttributeType::kInt2: {
      if (const auto updated = ui_int2_input(id, value.as_int2())) {
        return updated;
      }
      break;
    }
    case AttributeType::kInt3: {
      if (const auto updated = ui_int3_input(id, value.as_int3())) {
        return updated;
      }
      break;
    }
    case AttributeType::kInt4: {
      if (const auto updated = ui_int4_input(id, value.as_int4())) {
        return updated;
      }
      break;
    }
    case AttributeType::kFloat: {
      if (const auto updated = ui_float_input(id, value.as_float())) {
        return updated;
      }
      break;
    }
    case AttributeType::kFloat2: {
      if (const auto updated = ui_float2_input(id, value.as_float2())) {
        return updated;
      }
      break;
    }
    case AttributeType::kFloat3: {
      if (const auto updated = ui_float3_input(id, value.as_float3())) {
        return updated;
      }
      break;
    }
    case AttributeType::kFloat4: {
      if (const auto updated = ui_float4_input(id, value.as_float4())) {
        return updated;
      }
      break;
    }
    case AttributeType::kBool: {
      if (const auto updated = ui_bool_input(id, value.as_bool())) {
        return updated;
      }
      break;
    }
    case AttributeType::kPath: {
      if (auto updated = ui_file_path_input(id, value.as_path())) {
        return std::move(updated);
      }
      break;
    }
    case AttributeType::kColor: {
      if (const auto updated = ui_color_input(id, value.as_color())) {
        return updated;
      }
      break;
    }
    case AttributeType::kObject: {
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
  type_names[std::to_underlying(AttributeType::kStr)] = lang.misc.type_string.c_str();
  type_names[std::to_underlying(AttributeType::kInt)] = lang.misc.type_int.c_str();
  type_names[std::to_underlying(AttributeType::kInt2)] = lang.misc.type_int2.c_str();
  type_names[std::to_underlying(AttributeType::kInt3)] = lang.misc.type_int3.c_str();
  type_names[std::to_underlying(AttributeType::kInt4)] = lang.misc.type_int4.c_str();
  type_names[std::to_underlying(AttributeType::kFloat)] = lang.misc.type_float.c_str();
  type_names[std::to_underlying(AttributeType::kFloat2)] = lang.misc.type_float2.c_str();
  type_names[std::to_underlying(AttributeType::kFloat3)] = lang.misc.type_float3.c_str();
  type_names[std::to_underlying(AttributeType::kFloat4)] = lang.misc.type_float4.c_str();
  type_names[std::to_underlying(AttributeType::kBool)] = lang.misc.type_bool.c_str();
  type_names[std::to_underlying(AttributeType::kColor)] = lang.misc.type_color.c_str();
  type_names[std::to_underlying(AttributeType::kObject)] = lang.misc.type_object.c_str();
  type_names[std::to_underlying(AttributeType::kPath)] = lang.misc.type_path.c_str();

  const auto all_types = {AttributeType::kStr,
                          AttributeType::kInt,
                          AttributeType::kInt2,
                          AttributeType::kInt3,
                          AttributeType::kInt4,
                          AttributeType::kFloat,
                          AttributeType::kFloat2,
                          AttributeType::kFloat3,
                          AttributeType::kFloat4,
                          AttributeType::kBool,
                          AttributeType::kColor,
                          AttributeType::kObject,
                          AttributeType::kPath};

  const auto current_type_index = static_cast<usize>(std::to_underlying(current_type));
  const auto& current_type_name = type_names.at(current_type_index);

  Maybe<AttributeType> result;

  if (const Combo combo {"##AttributeTypeCombo", current_type_name.data()};
      combo.is_open()) {
    for (const auto type : all_types) {
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
