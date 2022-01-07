#include "property_type_combo.hpp"

#include <algorithm>  // find_if
#include <array>      // array
#include <cstring>    // strcmp
#include <utility>    // pair, make_pair

#include <imgui.h>
#include <tactile_def.hpp>
#include <tactile_stdlib.hpp>

#include "editor/gui/scoped.hpp"

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

void PropertyTypeComboImpl(PropertyType& out, Maybe<PropertyType> previousType)
{
  const auto currentIndex = GetIndexFromType(out);
  auto&& [currentName, currentType] = gItems.at(currentIndex);

  if (Scoped::Combo combo{"##PropertyTypeComboImpl", currentName}; combo.IsOpen()) {
    for (auto&& [name, type] : gItems) {
      Scoped::Disable disable{previousType == type};

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

void PropertyTypeCombo(PropertyType& out)
{
  PropertyTypeComboImpl(out, nothing);
}

void PropertyTypeCombo(const PropertyType previous, PropertyType& out)
{
  PropertyTypeComboImpl(out, previous);
}

}  // namespace Tactile
