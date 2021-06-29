#include "int_property_widget.hpp"

#include <limits>  // numeric_limits

#include "imgui.h"
#include "utils/scope_id.hpp"

namespace Tactile {

auto IntPropertyWidget(const Property& property) -> Maybe<int>
{
  const ScopeID id{&property};
  auto value = property.AsInteger();

  ImGui::SetNextItemWidth(-std::numeric_limits<float>::min());
  if (ImGui::DragInt("##IntPropertyInput", &value))
  {
    return value;
  }

  if (ImGui::IsItemHovered())
  {
    ImGui::SetTooltip("[int]");
  }

  return nothing;
}

}  // namespace Tactile
