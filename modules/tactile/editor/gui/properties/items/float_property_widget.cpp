#include "float_property_widget.hpp"

#include <limits>  // numeric_limits

#include <imgui.h>

#include "editor/gui/scoped.hpp"

namespace Tactile {

auto FloatPropertyWidget(const PropertyValue& property, const float min, const float max)
    -> Maybe<float>
{
  const Scoped::ID scope{&property};
  auto value = property.AsFloat();

  ImGui::SetNextItemWidth(-std::numeric_limits<float>::min());

  if (min != 0 || max != 0) {
    if (ImGui::SliderFloat("##FloatPropertyInput", &value, min, max)) {
      return value;
    }
  }
  else {
    if (ImGui::DragFloat("##FloatPropertyInput", &value)) {
      return value;
    }
  }

  if (ImGui::IsItemHovered()) {
    ImGui::SetTooltip("[float]");
  }

  return nothing;
}

}  // namespace Tactile
