#include "color_property_widget.hpp"

#include <imgui.h>

#include <cmath>  // round

#include "aliases/ints.hpp"
#include "utils/color_utils.hpp"
#include "utils/scope_id.hpp"

namespace Tactile {

auto ColorPropertyWidget(const Property& property) -> Maybe<cen::color>
{
  const ScopeID id{&property};

  constexpr auto flags = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel |
                         ImGuiColorEditFlags_AlphaBar;

  const auto& color = property.AsColor();
  auto colorArr = ColorToArray(color);

  if (ImGui::ColorEdit4("##ColorPropertyEdit", colorArr.data(), flags))
  {
    const auto red = static_cast<uint8>(std::round(colorArr.at(0) * 255.0f));
    const auto green = static_cast<uint8>(std::round(colorArr.at(1) * 255.0f));
    const auto blue = static_cast<uint8>(std::round(colorArr.at(2) * 255.0f));
    const auto alpha = static_cast<uint8>(std::round(colorArr.at(3) * 255.0f));

    return cen::color{red, green, blue, alpha};
  }

  if (ImGui::IsItemHovered())
  {
    ImGui::SetTooltip("[color]");
  }

  return nothing;
}

}  // namespace Tactile
