#include "property_type_combo.hpp"

#include <algorithm>  // find_if
#include <array>      // array
#include <cstring>    // strcmp
#include <utility>    // pair, make_pair

#include <imgui.h>

#include <tactile-base/tactile_error.hpp>
#include <tactile-base/tactile_std.hpp>

namespace Tactile {
namespace {

constexpr std::array items{std::make_pair("string", PropertyType::String),
                           std::make_pair("int", PropertyType::Integer),
                           std::make_pair("float", PropertyType::Floating),
                           std::make_pair("bool", PropertyType::Boolean),
                           std::make_pair("color", PropertyType::Color),
                           std::make_pair("object", PropertyType::Object),
                           std::make_pair("file", PropertyType::File)};

[[nodiscard]] auto GetIndexFromType(const PropertyType type) -> usize
{
  auto it = std::ranges::find_if(items, [=](const std::pair<CStr, PropertyType>& pair) {
    return type == pair.second;
  });

  if (it != items.end()) {
    return it - items.begin();
  }
  else {
    throw TactileError{"Invalid property type!"};
  }
}

void PropertyTypeComboImpl(PropertyType& out, Maybe<PropertyType> previous)
{
  const auto currentIndex = GetIndexFromType(out);
  auto&& [currentName, currentType] = items.at(currentIndex);

  if (ImGui::BeginCombo("##PropertyTypeComboImpl", currentName)) {
    for (auto&& [name, type] : items) {
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
