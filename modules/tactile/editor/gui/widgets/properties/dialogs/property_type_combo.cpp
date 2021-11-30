#include "property_type_combo.hpp"

#include <algorithm>  // find_if
#include <array>      // array
#include <cstring>    // strcmp
#include <utility>    // pair, make_pair

#include <tactile_def.hpp>
#include <tactile_stdlib.hpp>

#include <imgui.h>

namespace Tactile {
namespace {

constexpr std::array gItems{std::make_pair("string", PropertyType::String),
                            std::make_pair("int", PropertyType::Integer),
                            std::make_pair("float", PropertyType::Floating),
                            std::make_pair("bool", PropertyType::Boolean),
                            std::make_pair("color", PropertyType::Color),
                            std::make_pair("object", PropertyType::Object),
                            std::make_pair("file", PropertyType::File)};

[[nodiscard]] auto GetIndexFromType(const PropertyType type) -> usize
{
  const auto it = std::find_if(
      gItems.begin(),
      gItems.end(),
      [=](const std::pair<CStr, PropertyType>& pair) { return type == pair.second; });

  if (it != gItems.end()) {
    return static_cast<usize>(it - gItems.begin());
  }
  else {
    throw TactileError{"Invalid property type!"};
  }
}

void PropertyTypeComboImpl(PropertyType& out, Maybe<PropertyType> previous)
{
  const auto currentIndex = GetIndexFromType(out);
  auto&& [currentName, currentType] = gItems.at(currentIndex);

  if (ImGui::BeginCombo("##PropertyTypeComboImpl", currentName)) {
    for (auto&& [name, type] : gItems) {
      if (previous) {
        ImGui::BeginDisabled(type == previous);
      }

      const auto selected = std::strcmp(currentName, name) == 0;
      if (ImGui::Selectable(name, selected)) {
        out = type;
      }

      if (previous) {
        ImGui::EndDisabled();
      }

      if (selected) {
        ImGui::SetItemDefaultFocus();
      }
    }

    ImGui::EndCombo();
  }
}

}  // namespace

void PropertyTypeCombo(PropertyType& out)
{
  PropertyTypeComboImpl(out, nothing);
}

void PropertyTypeCombo(const PropertyType previous, PropertyType& out)
{
  PropertyTypeComboImpl(out, previous);
}

}  // namespace Tactile
