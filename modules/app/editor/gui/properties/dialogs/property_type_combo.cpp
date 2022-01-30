#include "property_type_combo.hpp"

#include <algorithm>  // find_if
#include <array>      // array
#include <cstring>    // strcmp
#include <utility>    // pair, make_pair

#include <imgui.h>

#include "editor/gui/scoped.hpp"
#include "tactile_def.hpp"
#include "tactile_stdlib.hpp"
#include "throw.hpp"

namespace tactile {
namespace {

constexpr std::array gItems{std::make_pair("string", attribute_type::string),
                            std::make_pair("int", attribute_type::integer),
                            std::make_pair("float", attribute_type::floating),
                            std::make_pair("bool", attribute_type::boolean),
                            std::make_pair("color", attribute_type::color),
                            std::make_pair("object", attribute_type::object),
                            std::make_pair("file", attribute_type::file)};

[[nodiscard]] auto GetIndexFromType(const attribute_type type) -> usize
{
  const auto it = std::find_if(
      gItems.begin(),
      gItems.end(),
      [=](const std::pair<c_str, attribute_type>& pair) { return type == pair.second; });

  if (it != gItems.end()) {
    return static_cast<usize>(it - gItems.begin());
  }
  else {
    ThrowTraced(TactileError{"Invalid property type!"});
  }
}

void PropertyTypeComboImpl(attribute_type& out, maybe<attribute_type> previousType)
{
  const auto currentIndex = GetIndexFromType(out);
  auto&& [currentName, currentType] = gItems.at(currentIndex);

  if (scoped::Combo combo{"##PropertyTypeComboImpl", currentName}; combo.IsOpen()) {
    for (auto&& [name, type] : gItems) {
      scoped::Disable disable{previousType == type};

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

void PropertyTypeCombo(attribute_type& out)
{
  PropertyTypeComboImpl(out, nothing);
}

void PropertyTypeCombo(const attribute_type previous, attribute_type& out)
{
  PropertyTypeComboImpl(out, previous);
}

}  // namespace tactile
