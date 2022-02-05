#include "add_property_dialog.hpp"

#include <imgui.h>

#include "core/systems/context_system.hpp"
#include "core/systems/property_system.hpp"
#include "core/utils/buffers.hpp"
#include "editor/events/property_events.hpp"
#include "editor/model.hpp"
#include "property_type_combo.hpp"

namespace tactile {

AddPropertyDialog::AddPropertyDialog() : ADialog{"Add Property"}
{
  SetAcceptButtonLabel("Add");
}

void AddPropertyDialog::Open()
{
  zero_buffer(mNameBuffer);
  mPropertyType = attribute_type::string;
  Show();
}

void AddPropertyDialog::UpdateContents(const Model&, entt::dispatcher&)
{
  ImGui::InputTextWithHint("##Name",
                           "Unique property name...",
                           mNameBuffer.data(),
                           sizeof mNameBuffer);
  PropertyTypeCombo(mPropertyType);
}

void AddPropertyDialog::OnAccept(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<AddPropertyEvent>(create_string_from_buffer(mNameBuffer),
                                       mPropertyType);
}

auto AddPropertyDialog::IsCurrentInputValid(const Model& model) const -> bool
{
  const auto& registry = model.GetActiveRegistryRef();
  const auto& context = sys::current_context(registry);

  const auto name = create_string_view_from_buffer(mNameBuffer);
  return !name.empty() && !sys::has_property_with_name(registry, context, name);
}

}  // namespace tactile