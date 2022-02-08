#include "add_property_dialog.hpp"

#include <imgui.h>

#include "core/systems/context_system.hpp"
#include "core/systems/property_system.hpp"
#include "core/utils/buffers.hpp"
#include "editor/events/property_events.hpp"
#include "editor/model.hpp"
#include "property_type_combo.hpp"

namespace tactile {

AddPropertyDialog::AddPropertyDialog() : dialog_base{"Add Property"}
{
  set_accept_button_label("Add");
}

void AddPropertyDialog::Open()
{
  zero_buffer(mNameBuffer);
  mPropertyType = attribute_type::string;
  make_visible();
}

void AddPropertyDialog::on_update(const Model&, entt::dispatcher&)
{
  ImGui::InputTextWithHint("##Name",
                           "Unique property name...",
                           mNameBuffer.data(),
                           sizeof mNameBuffer);
  PropertyTypeCombo(mPropertyType);
}

void AddPropertyDialog::on_accept(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<add_property_event>(create_string_from_buffer(mNameBuffer),
                                         mPropertyType);
}

auto AddPropertyDialog::is_current_input_valid(const Model& model) const -> bool
{
  const auto& registry = model.get_active_registry();
  const auto& context = sys::current_context(registry);

  const auto name = create_string_view_from_buffer(mNameBuffer);
  return !name.empty() && !sys::has_property_with_name(registry, context, name);
}

}  // namespace tactile
