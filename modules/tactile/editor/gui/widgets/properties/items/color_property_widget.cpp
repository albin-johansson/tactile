#include "color_property_widget.hpp"

#include <cmath>  // round

#include <tactile-base/tactile_std.hpp>

#include <imgui.h>

#include "core/utils/color_utils.hpp"
#include "core/utils/scope_id.hpp"

namespace Tactile {

auto ColorPropertyWidget(const PropertyValue& property) -> Maybe<cen::color>
{
  const ScopeID id{&property};

  constexpr auto flags = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel |
                         ImGuiColorEditFlags_AlphaBar;

  const auto& color = property.AsColor();
  auto array = ColorToArray(color);

  if (ImGui::ColorEdit4("##ColorPropertyEdit", array.data(), flags)) {
    return cen::color::from_norm(array.at(0), array.at(1), array.at(2), array.at(3));
  }

  if (ImGui::IsItemHovered()) {
    ImGui::SetTooltip("[color]");
  }

  return nothing;
}

}  // namespace Tactile
