#include "string_input_dialog.hpp"

#include <utility>  // move

#include <imgui.h>

#include "core/utils/buffers.hpp"
#include "editor/gui/common/button.hpp"

namespace tactile {

AStringInputDialog::AStringInputDialog(const c_str title) : dialog_base{title} {}

void AStringInputDialog::on_update(const Model&, entt::dispatcher&)
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

void AStringInputDialog::SetInputHint(const c_str hint)
{
  mHint = hint;
}

auto AStringInputDialog::is_current_input_valid(const Model& model) const -> bool
{
  return Validate(model, GetCurrentInput());
}

void AStringInputDialog::Show(std::string previous)
{
  mShouldAcquireFocus = true;
  mPrevious = std::move(previous);
  copy_string_into_buffer(mBuffer, mPrevious);
  make_visible();
}

auto AStringInputDialog::GetCurrentInput() const -> std::string_view
{
  return create_string_view_from_buffer(mBuffer);
}

auto AStringInputDialog::GetPreviousString() const -> const std::string&
{
  return mPrevious;
}

}  // namespace tactile
