#include "dialog.hpp"

#include "editor/gui/alignment.hpp"
#include "editor/gui/common/button.hpp"
#include "editor/gui/scoped.hpp"

namespace Tactile {

ADialog::ADialog(const CStr title) : mTitle{title} {}

void ADialog::Update(const Model& model, entt::dispatcher& dispatcher)
{
  if (mShow) {
    ImGui::OpenPopup(mTitle);
    mShow = false;
  }

  CenterNextWindowOnAppearance();

  constexpr auto flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;
  Scoped::Modal modal{mTitle, flags};
  if (modal.IsOpen()) {
    UpdateContents(model, dispatcher);

    ImGui::Spacing();

    if (ImGui::Button("Cancel")) {
      OnCancel();
      ImGui::CloseCurrentPopup();
    }

    const auto valid = IsCurrentInputValid(model);

    ImGui::SameLine();

    if (Button(mAcceptButtonLabel, nullptr, valid)) {
      OnAccept(dispatcher);
      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();

    if (mApplyButtonLabel && Button(mApplyButtonLabel, nullptr, valid)) {
      OnApply(dispatcher);
    }
  }
}

void ADialog::Show()
{
  mShow = true;
}

void ADialog::SetAcceptButtonLabel(const CStr label)
{
  mAcceptButtonLabel = label;
}

void ADialog::SetApplyButtonLabel(const CStr label)
{
  mApplyButtonLabel = label;
}

}  // namespace Tactile