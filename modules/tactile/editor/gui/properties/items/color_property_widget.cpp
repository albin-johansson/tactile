#include "color_property_widget.hpp"

#include <cmath>  // round

#include <tactile_def.hpp>

#include <imgui.h>

#include "core/utils/color_utils.hpp"
#include "editor/gui/scoped.hpp"

namespace Tactile {

auto ColorPropertyWidget(const PropertyValue& property) -> Maybe<cen::Color>
{
  const Scoped::ID scope{&property};

  constexpr auto flags = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel |
                         ImGuiColorEditFlags_AlphaBar;

  const auto& color = property.AsColor();
  auto array = ColorToArray(color);

  if (ImGui::ColorEdit4("##ColorPropertyEdit", array.data(), flags)) {
    return cen::Color::FromNorm(array.at(0), array.at(1), array.at(2), array.at(3));
  }

  if (ImGui::IsItemHovered()) {
    ImGui::SetTooltip("[color]");
  }

  return nothing;
}

}  // namespace Tactile
