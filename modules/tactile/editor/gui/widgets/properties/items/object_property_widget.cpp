#include "object_property_widget.hpp"

#include <imgui.h>

#include "editor/gui/widgets/scoped.hpp"

namespace Tactile {

auto ObjectPropertyWidget(const PropertyValue& property) -> Maybe<ObjectRef>
{
  const Scoped::ID scope{&property};

  // TODO
  const auto object = property.AsObject();
  ImGui::Text("%i", object);

  if (ImGui::IsItemHovered()) {
    ImGui::SetTooltip("[object]");
  }

  return nothing;
}

}  // namespace Tactile
