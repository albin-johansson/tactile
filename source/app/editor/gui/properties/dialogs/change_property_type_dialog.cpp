#include "change_property_type_dialog.hpp"

#include <utility>  // move

#include <imgui.h>

#include "editor/events/property_events.hpp"
#include "editor/gui/common/button.hpp"
#include "property_type_combo.hpp"

namespace tactile {

ChangePropertyTypeDialog::ChangePropertyTypeDialog() : dialog_base{"Change Property Type"}
{
  set_accept_button_label("Change");
}

void ChangePropertyTypeDialog::Show(std::string name, const attribute_type type)
{
  mPropertyName = std::move(name);
  mCurrentType = type;
  mPreviousType = type;
  make_visible();
}

void ChangePropertyTypeDialog::on_update(const Model&, entt::dispatcher&)
{
  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted("Type: ");

  ImGui::SameLine();
  PropertyTypeCombo(mPreviousType.value(), mCurrentType);
}

void ChangePropertyTypeDialog::on_accept(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<change_property_type_event>(mPropertyName.value(), mCurrentType);
}

auto ChangePropertyTypeDialog::is_current_input_valid(const Model&) const -> bool
{
  return mCurrentType != mPreviousType.value();
}

}  // namespace tactile
