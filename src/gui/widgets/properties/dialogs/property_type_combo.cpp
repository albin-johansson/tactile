#include "property_type_combo.hpp"

#include <imgui.h>

#include "core/tactile_error.hpp"

namespace Tactile {

auto PropertyTypeCombo(int* index) -> bool
{
  constexpr auto options = "string\0int\0float\0bool\0color\0object\0file\0\0";
  return ImGui::Combo("##PropertyTypeCombo", index, options);
}

auto GetPropertyTypeFromComboIndex(const int index) -> PropertyType
{
  switch (index)
  {
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
