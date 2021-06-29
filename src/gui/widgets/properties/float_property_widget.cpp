#include "float_property_widget.hpp"

#include <limits>  // numeric_limits

#include "imgui.h"
#include "utils/scope_id.hpp"

namespace Tactile {

auto FloatPropertyWidget(const Property& property) -> Maybe<float>
{
  const ScopeID id{&property};
  auto value = property.AsFloating();

  ImGui::SetNextItemWidth(-std::numeric_limits<float>::min());
  if (ImGui::DragFloat("##FloatPropertyInput", &value))
  {
    return value;
  }

  if (ImGui::IsItemHovered())
  {
    ImGui::SetTooltip("[float]");
  }

  return nothing;
}

}  // namespace Tactile
