#include "change_property_type_dialog.hpp"

#include <utility>  // move

#include <imgui.h>

#include "editor/events/property_events.hpp"
#include "editor/gui/common/button.hpp"
#include "property_type_combo.hpp"

namespace Tactile {

ChangePropertyTypeDialog::ChangePropertyTypeDialog() : ADialog{"Change Property Type"}
{
  SetAcceptButtonLabel("Change");
}

void ChangePropertyTypeDialog::Show(std::string name, const PropertyType type)
{
  mPropertyName = std::move(name);
  mCurrentType = type;
  mPreviousType = type;
  ADialog::Show();
}

void ChangePropertyTypeDialog::UpdateContents(const entt::registry&, entt::dispatcher&)
{
  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted("Type: ");

  ImGui::SameLine();
  PropertyTypeCombo(mPreviousType.value(), mCurrentType);
}

void ChangePropertyTypeDialog::OnAccept(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<ChangePropertyTypeEvent>(mPropertyName.value(), mCurrentType);
}

auto ChangePropertyTypeDialog::IsCurrentInputValid(const entt::registry&) const -> bool
{
  return mCurrentType != mPreviousType.value();
}

}  // namespace Tactile
