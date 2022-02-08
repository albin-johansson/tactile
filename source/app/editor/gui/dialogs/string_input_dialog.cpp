#include "string_input_dialog.hpp"

#include <utility>  // move

#include <imgui.h>

#include "core/utils/buffers.hpp"

namespace tactile {

string_input_dialog::string_input_dialog(const char* title) : dialog_base{title} {}

void string_input_dialog::show(std::string previous)
{
  mShouldAcquireFocus = true;
  mPrevious = std::move(previous);
  copy_string_into_buffer(mBuffer, mPrevious);
  make_visible();
}

void string_input_dialog::set_input_hint(const char* hint)
{
  mHint = hint;
}

void string_input_dialog::on_update(const Model&, entt::dispatcher&)
{
  if (mShouldAcquireFocus) {
    ImGui::SetKeyboardFocusHere();
    mShouldAcquireFocus = false;
  }
  ImGui::InputTextWithHint("##string_input_dialog_input",
                           mHint ? mHint : "",
                           mBuffer.data(),
                           sizeof mBuffer);
}

auto string_input_dialog::is_current_input_valid(const Model& model) const -> bool
{
  return validate(model, current_input());
}

auto string_input_dialog::current_input() const -> std::string_view
{
  return create_string_view_from_buffer(mBuffer);
}

auto string_input_dialog::previous_input() const -> const std::string&
{
  return mPrevious;
}

}  // namespace tactile
