#include "dialog.hpp"

#include "editor/gui/alignment.hpp"
#include "editor/gui/common/button.hpp"
#include "editor/gui/scoped.hpp"
#include "misc/throw.hpp"

namespace tactile {

dialog_base::dialog_base(const c_str title) : mTitle{title}
{
  if (!mTitle) {
    throw_traced(tactile_error{"Invalid null dialog title!"});
  }
}

void dialog_base::update(const document_model& model, entt::dispatcher& dispatcher)
{
  if (mShow) {
    ImGui::OpenPopup(mTitle);
    mShow = false;
  }

  center_next_window_on_appearance();

  constexpr auto flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;
  if (scoped::modal modal{mTitle, flags}; modal.is_open()) {
    on_update(model, dispatcher);

    ImGui::Spacing();

    if (mCloseButtonLabel && button(mCloseButtonLabel)) {
      on_cancel();
      ImGui::CloseCurrentPopup();
    }

    const auto valid = is_current_input_valid(model);

    if (mAcceptButtonLabel) {
      if (mCloseButtonLabel) {
        ImGui::SameLine();
      }

      if (button(mAcceptButtonLabel, nullptr, valid)) {
        on_accept(dispatcher);
        ImGui::CloseCurrentPopup();
      }
    }

    if (mUseApplyButton) {
      if (mCloseButtonLabel || mAcceptButtonLabel) {
        ImGui::SameLine();
      }
      if (button("Apply", nullptr, valid)) {
        on_apply(dispatcher);
      }
    }
  }
}

void dialog_base::make_visible()
{
  mShow = true;
}

void dialog_base::use_apply_button()
{
  mUseApplyButton = true;
}

void dialog_base::set_accept_button_label(const c_str label)
{
  mAcceptButtonLabel = label;
}

void dialog_base::set_close_button_label(const c_str label)
{
  mCloseButtonLabel = label;
}

}  // namespace tactile