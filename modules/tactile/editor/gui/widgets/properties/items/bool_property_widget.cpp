#include "bool_property_widget.hpp"

#include <imgui.h>

#include "core/utils/scope_id.hpp"

namespace Tactile {

auto BoolPropertyWidget(const PropertyValue& property) -> Maybe<bool>
{
  const ScopeID id{&property};
  auto value = property.AsBool();

  if (ImGui::Checkbox("##BoolPropertyWidget", &value)) {
    return value;
  }

  if (ImGui::IsItemHovered()) {
    ImGui::SetTooltip("[bool]");
  }

  return nothing;
}

}  // namespace Tactile
