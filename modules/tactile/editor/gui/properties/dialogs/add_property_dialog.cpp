#include "add_property_dialog.hpp"

#include <imgui.h>

#include "core/systems/property_system.hpp"
#include "core/utils/buffer_utils.hpp"
#include "editor/events/property_events.hpp"
#include "property_type_combo.hpp"

namespace Tactile {

AddPropertyDialog::AddPropertyDialog() : ADialog{"Add Property"}
{
  SetAcceptButtonLabel("Add");
}

void AddPropertyDialog::UpdateContents(const entt::registry&, entt::dispatcher&)
{
  ImGui::InputTextWithHint("##Name",
                           "Unique property name...",
                           mNameBuffer.data(),
                           sizeof mNameBuffer);
  PropertyTypeCombo(mPropertyType);
}

void AddPropertyDialog::OnAccept(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<AddPropertyEvent>(CreateStringFromBuffer(mNameBuffer),
                                       mPropertyType);
}

auto AddPropertyDialog::IsCurrentInputValid(const entt::registry& registry) const -> bool
{
  const auto name = CreateStringViewFromBuffer(mNameBuffer);
  const auto& context = Sys::GetCurrentContext(registry);
  return !name.empty() && !Sys::HasPropertyWithName(registry, context, name);
}

}  // namespace Tactile
