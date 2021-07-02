#include "object_property_widget.hpp"

#include <imgui.h>

#include "utils/scope_id.hpp"

namespace Tactile {

auto ObjectPropertyWidget(const Property& property) -> Maybe<object_ref>
{
  const ScopeID id{&property};

  // TODO
  const auto object = property.AsObject();
  ImGui::Text("%i", object.get());

  if (ImGui::IsItemHovered())
  {
    ImGui::SetTooltip("[object]");
  }

  return nothing;
}

}  // namespace Tactile
