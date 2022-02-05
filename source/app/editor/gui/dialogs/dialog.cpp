#include "dialog.hpp"

#include "editor/gui/alignment.hpp"
#include "editor/gui/common/button.hpp"
#include "editor/gui/scoped.hpp"

namespace tactile {

ADialog::ADialog(const c_str title) : mTitle{title} {}

void ADialog::Update(const Model& model, entt::dispatcher& dispatcher)
{
  if (mShow) {
    ImGui::OpenPopup(mTitle);
    mShow = false;
  }

  center_next_window_on_appearance();

  constexpr auto flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;
  if (scoped::modal modal{mTitle, flags}; modal.is_open()) {
    UpdateContents(model, dispatcher);

    ImGui::Spacing();

    if (mCloseButtonLabel && button(mCloseButtonLabel)) {
      OnCancel();
      ImGui::CloseCurrentPopup();
    }

    const auto valid = IsCurrentInputValid(model);

    if (mAcceptButtonLabel) {
      if (mCloseButtonLabel) {
        ImGui::SameLine();
      }

      if (button(mAcceptButtonLabel, nullptr, valid)) {
        OnAccept(dispatcher);
        ImGui::CloseCurrentPopup();
      }
    }

    if (mApplyButtonLabel) {
      if (mCloseButtonLabel || mAcceptButtonLabel) {
        ImGui::SameLine();
      }
      if (button(mApplyButtonLabel, nullptr, valid)) {
        OnApply(dispatcher);
      }
    }
  }
}

void ADialog::Show()
{
  mShow = true;
}

void ADialog::SetAcceptButtonLabel(const c_str label)
{
  mAcceptButtonLabel = label;
}

void ADialog::SetApplyButtonLabel(const c_str label)
{
  mApplyButtonLabel = label;
}

void ADialog::SetCloseButtonLabel(const c_str label)
{
  mCloseButtonLabel = label;
}

}  // namespace tactile