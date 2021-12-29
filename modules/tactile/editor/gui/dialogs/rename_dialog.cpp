#include "rename_dialog.hpp"

#include <utility>  // move

#include <imgui.h>

#include "core/utils/buffer_utils.hpp"
#include "editor/gui/common/button.hpp"

namespace Tactile {

ARenameDialog::ARenameDialog(const CStr title) : ADialog{title}
{
  SetAcceptButtonLabel("Rename");
}

void ARenameDialog::UpdateContents(const Model&, entt::dispatcher&)
{
  ImGui::InputText("##NameInput", mNameBuffer.data(), sizeof mNameBuffer);
  ImGui::SetItemDefaultFocus();
}

auto ARenameDialog::IsCurrentInputValid(const Model& model) const -> bool
{
  return Validate(model, GetCurrentInput());
}

void ARenameDialog::Show(std::string oldName)
{
  mOldName = std::move(oldName);
  CopyStringIntoBuffer(mNameBuffer, mOldName);
  ADialog::Show();
}

auto ARenameDialog::GetCurrentInput() const -> std::string_view
{
  return CreateStringViewFromBuffer(mNameBuffer);
}

auto ARenameDialog::GetPreviousName() const -> const std::string&
{
  return mOldName;
}

}  // namespace Tactile
