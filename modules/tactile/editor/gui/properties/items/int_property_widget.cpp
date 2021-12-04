#include "int_property_widget.hpp"

#include <limits>  // numeric_limits

#include <imgui.h>

#include "editor/gui/scoped.hpp"

namespace Tactile {

auto IntPropertyWidget(const PropertyValue& property) -> Maybe<int>
{
  const Scoped::ID scope{&property};
  auto value = property.AsInt();

  ImGui::SetNextItemWidth(-std::numeric_limits<float>::min());
  if (ImGui::DragInt("##IntPropertyInput", &value)) {
    return value;
  }

  if (ImGui::IsItemHovered()) {
    ImGui::SetTooltip("[int]");
  }

  return nothing;
}

}  // namespace Tactile
