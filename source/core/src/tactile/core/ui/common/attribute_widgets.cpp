// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/common/attribute_widgets.hpp"

#include <algorithm>  // copy_n, min
#include <array>      // array
#include <cstring>    // strlen
#include <utility>    // to_underlying

#include <imgui.h>

#include "tactile/base/int.hpp"
#include "tactile/core/meta/color.hpp"
#include "tactile/core/platform/file_dialog.hpp"
#include "tactile/core/ui/common/buttons.hpp"
#include "tactile/core/ui/common/widgets.hpp"
#include "tactile/core/ui/i18n/language.hpp"

namespace tactile::ui {
inline namespace attribute_widgets {

using AttributeTypeNameArray = std::array<const char*, 13>;

inline constexpr usize kStringCharLimit = 128;

auto _push_attribute_type_selectable(const AttributeTypeNameArray& names,
                                     const AttributeType selectable_type,
                                     const AttributeType current_type) -> bool
{
  const auto is_selected = current_type == selectable_type;
  const auto& type_name = names.at(static_cast<usize>(selectable_type));

  const auto pressed = ImGui::Selectable(type_name, is_selected);

  if (is_selected) {
    ImGui::SetItemDefaultFocus();
  }

  return pressed;
}

}  // namespace attribute_widgets

auto push_string_input(const char* id, const Attribute::string_type& str)
    -> std::optional<Attribute::string_type>
{
  const IdScope scope {id};
  std::optional<Attribute::string_type> new_str {};

  std::array<char, kStringCharLimit + 1> buffer;  // NOLINT

  // Make sure we leave one character available for the NUL terminator.
  const auto copy_count = std::min(str.size(), buffer.size() - 1);

  auto copy_end = std::copy_n(str.begin(), copy_count, buffer.begin());
  *copy_end = '\0';

  ImGui::SetNextItemWidth(-1.0f);
  if (ImGui::InputText("##Str", buffer.data(), buffer.size())) {
    const auto new_str_length = std::strlen(buffer.data());
    new_str = Attribute::string_type {buffer.data(), new_str_length};
  }

  return new_str;
}

auto push_int_input(const char* id,
                    Attribute::int_type value) -> std::optional<Attribute::int_type>
{
  const IdScope scope {id};
  std::optional<Attribute::int_type> new_value {};

  ImGui::SetNextItemWidth(-1.0f);
  if (ImGui::DragInt("##Int", &value)) {
    new_value = value;
  }

  return new_value;
}

auto push_int2_input(const char* id,
                     Attribute::int2_type value) -> std::optional<Attribute::int2_type>
{
  const IdScope scope {id};
  std::optional<Attribute::int2_type> new_value {};

  ImGui::SetNextItemWidth(-1.0f);
  if (ImGui::DragInt2("##Int2", value.data())) {
    new_value = value;
  }

  return new_value;
}

auto push_int3_input(const char* id,
                     Attribute::int3_type value) -> std::optional<Attribute::int3_type>
{
  const IdScope scope {id};
  std::optional<Attribute::int3_type> new_value {};

  ImGui::SetNextItemWidth(-1.0f);
  if (ImGui::DragInt3("##Int3", value.data())) {
    new_value = value;
  }

  return new_value;
}

auto push_int4_input(const char* id,
                     Attribute::int4_type value) -> std::optional<Attribute::int4_type>
{
  const IdScope scope {id};
  std::optional<Attribute::int4_type> new_value {};

  ImGui::SetNextItemWidth(-1.0f);
  if (ImGui::DragInt4("##Int4", value.data())) {
    new_value = value;
  }

  return new_value;
}

auto push_float_input(const char* id,
                      Attribute::float_type value) -> std::optional<Attribute::float_type>
{
  const IdScope scope {id};
  std::optional<Attribute::float_type> new_value {};

  ImGui::SetNextItemWidth(-1.0f);
  if (ImGui::DragFloat("##Float", &value)) {
    new_value = value;
  }

  return new_value;
}

auto push_float2_input(const char* id,
                       Attribute::float2_type value) -> std::optional<Attribute::float2_type>
{
  const IdScope scope {id};
  std::optional<Attribute::float2_type> new_value {};

  ImGui::SetNextItemWidth(-1.0f);
  if (ImGui::DragFloat2("##Float2", value.data())) {
    new_value = value;
  }

  return new_value;
}

auto push_float3_input(const char* id,
                       Attribute::float3_type value) -> std::optional<Attribute::float3_type>
{
  const IdScope scope {id};
  std::optional<Attribute::float3_type> new_value {};

  ImGui::SetNextItemWidth(-1.0f);
  if (ImGui::DragFloat3("##Float3", value.data())) {
    new_value = value;
  }

  return new_value;
}

auto push_float4_input(const char* id,
                       Attribute::float4_type value) -> std::optional<Attribute::float4_type>
{
  const IdScope scope {id};
  std::optional<Attribute::float4_type> new_value {};

  ImGui::SetNextItemWidth(-1.0f);
  if (ImGui::DragFloat4("##Float4", value.data())) {
    new_value = value;
  }

  return new_value;
}

auto push_bool_input(const char* id, bool value) -> std::optional<bool>
{
  const IdScope scope {id};
  std::optional<bool> new_value {};

  if (ImGui::Checkbox("##Bool", &value)) {
    new_value = value;
  }

  return new_value;
}

auto push_path_input(const char* id,
                     const Attribute::path_type& path) -> std::optional<Attribute::path_type>
{
  const IdScope scope {id};
  std::optional<Attribute::path_type> new_path {};

#if TACTILE_OS_WINDOWS
  const auto path_string = path.string();
  const auto* path_c_str = path_string.c_str();
#else
  const auto* path_c_str = path.c_str();
#endif

  if (push_icon_button(Icon::kEllipsis)) {
    if (auto selected_path = FileDialog::open_file()) {
      new_path = Attribute::path_type {std::move(*selected_path)};
    }
  }

  ImGui::SameLine();

  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted(path_c_str);

  return new_path;
}

auto push_color_input(const char* id, const Attribute::color_type& color)
    -> std::optional<Attribute::color_type>
{
  const IdScope scope {id};
  std::optional<Attribute::color_type> new_color {};

  const auto flags = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel |
                     ImGuiColorEditFlags_AlphaBar;

  auto rgba_array = normalize(color);
  if (ImGui::ColorEdit4("##Color", &rgba_array.red, flags)) {
    new_color = make_color(rgba_array);
  }

  return new_color;
}

auto push_objref_input(const char* id, const Attribute::objref_type& object)
    -> std::optional<Attribute::objref_type>
{
  const IdScope scope {id};
  std::optional<Attribute::objref_type> new_object {};

  // TODO
  ImGui::Text("%i", object.value);

  return new_object;
}

// NOLINTNEXTLINE(*-cognitive-complexity)
auto push_attribute_input(const char* id,
                          const Attribute& attribute) -> std::optional<Attribute>
{
  switch (attribute.get_type()) {
    case AttributeType::kStr: {
      if (auto new_value = push_string_input(id, attribute.as_string())) {
        return Attribute {*new_value};
      }
      break;
    }
    case AttributeType::kInt: {
      if (auto new_value = push_int_input(id, attribute.as_int())) {
        return Attribute {*new_value};
      }
      break;
    }
    case AttributeType::kInt2: {
      if (auto new_value = push_int2_input(id, attribute.as_int2())) {
        return Attribute {*new_value};
      }
      break;
    }
    case AttributeType::kInt3: {
      if (auto new_value = push_int3_input(id, attribute.as_int3())) {
        return Attribute {*new_value};
      }
      break;
    }
    case AttributeType::kInt4: {
      if (auto new_value = push_int4_input(id, attribute.as_int4())) {
        return Attribute {*new_value};
      }
      break;
    }
    case AttributeType::kFloat: {
      if (auto new_value = push_float_input(id, attribute.as_float())) {
        return Attribute {*new_value};
      }
      break;
    }
    case AttributeType::kFloat2: {
      if (auto new_value = push_float2_input(id, attribute.as_float2())) {
        return Attribute {*new_value};
      }
      break;
    }
    case AttributeType::kFloat3: {
      if (auto new_value = push_float3_input(id, attribute.as_float3())) {
        return Attribute {*new_value};
      }
      break;
    }
    case AttributeType::kFloat4: {
      if (auto new_value = push_float4_input(id, attribute.as_float4())) {
        return Attribute {*new_value};
      }
      break;
    }
    case AttributeType::kBool: {
      if (auto new_value = push_bool_input(id, attribute.as_bool())) {
        return Attribute {*new_value};
      }
      break;
    }
    case AttributeType::kPath: {
      if (auto new_value = push_path_input(id, attribute.as_path())) {
        return Attribute {*new_value};
      }
      break;
    }
    case AttributeType::kColor: {
      if (auto new_value = push_color_input(id, attribute.as_color())) {
        return Attribute {*new_value};
      }
      break;
    }
    case AttributeType::kObject: {
      if (auto new_value = push_objref_input(id, attribute.as_object())) {
        return Attribute {*new_value};
      }
      break;
    }
  }

  return std::nullopt;
}

auto push_attribute_type_combo(const Language& language,
                               const char* label,
                               AttributeType& type) -> bool
{
  AttributeTypeNameArray type_names {};

  // clang-format off
  type_names[std::to_underlying(AttributeType::kStr)] = language.get(StringID::kString);
  type_names[std::to_underlying(AttributeType::kInt)] = language.get(StringID::kInt);
  type_names[std::to_underlying(AttributeType::kInt2)] = language.get(StringID::kInt2);
  type_names[std::to_underlying(AttributeType::kInt3)] = language.get(StringID::kInt3);
  type_names[std::to_underlying(AttributeType::kInt4)] = language.get(StringID::kInt4);
  type_names[std::to_underlying(AttributeType::kFloat)] = language.get(StringID::kFloat);
  type_names[std::to_underlying(AttributeType::kFloat2)] = language.get(StringID::kFloat2);
  type_names[std::to_underlying(AttributeType::kFloat3)] = language.get(StringID::kFloat3);
  type_names[std::to_underlying(AttributeType::kFloat4)] = language.get(StringID::kFloat4);
  type_names[std::to_underlying(AttributeType::kBool)] = language.get(StringID::kBool);
  type_names[std::to_underlying(AttributeType::kColor)] = language.get(StringID::kColor);
  type_names[std::to_underlying(AttributeType::kObject)] = language.get(StringID::kObject);
  type_names[std::to_underlying(AttributeType::kPath)] = language.get(StringID::kPath);
  // clang-format on

  bool changed {false};

  const auto& preview_name = type_names.at(static_cast<usize>(type));
  if (const ComboScope type_combo {label, preview_name}; type_combo.is_open()) {
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

    for (const auto other_type : all_types) {
      const DisabledScope disabled_if {type == other_type};
      if (_push_attribute_type_selectable(type_names, other_type, type)) {
        type = other_type;
        changed = true;
      }
    }
  }

  return changed;
}

}  // namespace tactile::ui
