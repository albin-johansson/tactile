#include "change_property_type_dialog.hpp"

#include <utility>  // move

#include <imgui.h>

#include "editor/events/property_events.hpp"
#include "editor/gui/common/button.hpp"
#include "property_type_combo.hpp"

namespace tactile {

ChangePropertyTypeDialog::ChangePropertyTypeDialog() : ADialog{"Change Property Type"}
{
  SetAcceptButtonLabel("Change");
}

void ChangePropertyTypeDialog::Show(std::string name, const attribute_type type)
{
  mPropertyName = std::move(name);
  mCurrentType = type;
  mPreviousType = type;
  ADialog::Show();
}

void ChangePropertyTypeDialog::UpdateContents(const Model&, entt::dispatcher&)
{
  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted("Type: ");

  ImGui::SameLine();
  PropertyTypeCombo(mPreviousType.value(), mCurrentType);
}

void ChangePropertyTypeDialog::OnAccept(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<change_property_type_event>(mPropertyName.value(), mCurrentType);
}

auto ChangePropertyTypeDialog::IsCurrentInputValid(const Model&) const -> bool
{
  return mCurrentType != mPreviousType.value();
}

}  // namespace tactile
