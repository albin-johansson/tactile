#include "bool_property_widget.hpp"

#include <imgui.h>

#include "editor/gui/widgets/scoped.hpp"

namespace Tactile {

auto BoolPropertyWidget(const PropertyValue& property) -> Maybe<bool>
{
  const Scoped::ID scope{&property};
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
