#include "property_type_combo.hpp"

#include <imgui.h>

#include <array>    // array
#include <cstring>  // strcmp

#include "common/cstr.hpp"
#include "common/ints.hpp"
#include "common/maybe.hpp"
#include "core/tactile_error.hpp"

namespace Tactile {
namespace {

constexpr std::array items{"string", "int", "float", "bool", "color", "object", "file"};

[[nodiscard]] auto StringToType(const CStr str) -> PropertyType
{
  if (std::strcmp(str, "string") == 0) {
    return PropertyType::String;
  }
  else if (std::strcmp(str, "int") == 0) {
    return PropertyType::Integer;
  }
  else if (std::strcmp(str, "float") == 0) {
    return PropertyType::Floating;
  }
  else if (std::strcmp(str, "bool") == 0) {
    return PropertyType::Boolean;
  }
  else if (std::strcmp(str, "color") == 0) {
    return PropertyType::Color;
  }
  else if (std::strcmp(str, "object") == 0) {
    return PropertyType::Object;
  }
  else if (std::strcmp(str, "file") == 0) {
    return PropertyType::File;
  }
  else {
    throw TactileError{"Invalid property type name!"};
  }
}

[[nodiscard]] auto TypeToIndex(const PropertyType type) -> usize
{
  switch (type) {
    case PropertyType::String:
      return 0;

    case PropertyType::Integer:
      return 1;

    case PropertyType::Floating:
      return 2;

    case PropertyType::Boolean:
      return 3;

    case PropertyType::Color:
      return 4;

    case PropertyType::Object:
      return 5;

    case PropertyType::File:
      return 6;

    default:
      throw TactileError{"Invalid property type!"};
  }
}

void PropertyTypeCombo(const Maybe<PropertyType> previous, PropertyType& out)
{
  const auto currentIndex = TypeToIndex(out);
  const auto* currentName = items.at(currentIndex);

  if (ImGui::BeginCombo("##PropertyTypeCombo", currentName)) {
    for (const auto* item : items) {
      const auto itemType = StringToType(item);

      ImGui::BeginDisabled(itemType == previous);

      const auto selected = std::strcmp(currentName, item) == 0;
      if (ImGui::Selectable(item, selected)) {
        out = itemType;
      }

      ImGui::EndDisabled();

      if (selected) {
        ImGui::SetItemDefaultFocus();
      }
    }

    ImGui::EndCombo();
  }
}

}  // namespace

void PropertyTypeCombo(const PropertyType previous, PropertyType& out)
{
  const auto currentIndex = TypeToIndex(out);
  const auto* currentName = items.at(currentIndex);

  if (ImGui::BeginCombo("##PropertyTypeCombo", currentName)) {
    for (const auto* item : items) {
      const auto itemType = StringToType(item);

      ImGui::BeginDisabled(itemType == previous);

      const auto selected = std::strcmp(currentName, item) == 0;
      if (ImGui::Selectable(item, selected)) {
        out = itemType;
      }

      ImGui::EndDisabled();

      if (selected) {
        ImGui::SetItemDefaultFocus();
      }
    }

    ImGui::EndCombo();
  }
}

auto PropertyTypeCombo(int* index) -> bool
{
  constexpr auto options = "string\0int\0float\0bool\0color\0object\0file\0\0";
  return ImGui::Combo("##PropertyTypeCombo", index, options);
}

auto GetPropertyTypeFromComboIndex(const int index) -> PropertyType
{
  switch (index) {
    case 0:
      return PropertyType::String;

    case 1:
      return PropertyType::Integer;

    case 2:
      return PropertyType::Floating;

    case 3:
      return PropertyType::Boolean;

    case 4:
      return PropertyType::Color;

    case 5:
      return PropertyType::Object;

    case 6:
      return PropertyType::File;

    default:
      throw TactileError{"Invalid property type index!"};
  }
}

}  // namespace Tactile
