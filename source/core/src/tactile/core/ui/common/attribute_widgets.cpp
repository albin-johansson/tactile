// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/common/attribute_widgets.hpp"

#include <utility>  // to_underlying

#include "tactile/base/container/array.hpp"
#include "tactile/core/ui/common/widgets.hpp"
#include "tactile/core/ui/i18n/language.hpp"

namespace tactile::ui {
inline namespace attribute_widgets {

using AttributeTypeNameArray = Array<const char*, 13>;

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
