#include "string_input_dialog.hpp"

#include <utility>  // move

#include <imgui.h>

#include "core/utils/buffer_utils.hpp"
#include "editor/gui/common/button.hpp"

namespace Tactile {

AStringInputDialog::AStringInputDialog(const CStr title) : ADialog{title} {}

void AStringInputDialog::UpdateContents(const Model&, entt::dispatcher&)
{
  if (mShouldAcquireFocus) {
    ImGui::SetKeyboardFocusHere();
    mShouldAcquireFocus = false;
  }
  ImGui::InputTextWithHint("##AStringInputDialogInput",
                           mHint ? mHint : "",
                           mBuffer.data(),
                           sizeof mBuffer);
}

void AStringInputDialog::SetInputHint(const CStr hint)
{
  mHint = hint;
}

auto AStringInputDialog::IsCurrentInputValid(const Model& model) const -> bool
{
  return Validate(model, GetCurrentInput());
}

void AStringInputDialog::Show(std::string previous)
{
  mShouldAcquireFocus = true;
  mPrevious = std::move(previous);
  CopyStringIntoBuffer(mBuffer, mPrevious);
  ADialog::Show();
}

auto AStringInputDialog::GetCurrentInput() const -> std::string_view
{
  return CreateStringViewFromBuffer(mBuffer);
}

auto AStringInputDialog::GetPreviousString() const -> const std::string&
{
  return mPrevious;
}

}  // namespace Tactile