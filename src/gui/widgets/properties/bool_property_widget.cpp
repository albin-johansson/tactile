#include "bool_property_widget.hpp"

#include <imgui.h>

#include "utils/scope_id.hpp"

namespace Tactile {

auto BoolPropertyWidget(const Property& property) -> Maybe<bool>
{
  const ScopeID id{&property};
  auto value = property.AsBoolean();

  if (ImGui::Checkbox("##asd", &value))
  {
    return value;
  }

  if (ImGui::IsItemHovered())
  {
    ImGui::SetTooltip("[bool]");
  }

  return nothing;
}

}  // namespace Tactile
