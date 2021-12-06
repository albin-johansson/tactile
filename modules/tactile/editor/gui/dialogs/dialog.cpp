#include "dialog.hpp"

#include "editor/gui/alignment.hpp"
#include "editor/gui/common/button.hpp"
#include "editor/gui/scoped.hpp"

namespace Tactile {

ADialog::ADialog(const CStr title) : mTitle{title} {}

void ADialog::Update(const entt::registry& registry, entt::dispatcher& dispatcher)
{
  if (mShow) {
    ImGui::OpenPopup(mTitle);
    mShow = false;
  }

  CenterNextWindowOnAppearance();

  constexpr auto flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;
  Scoped::Modal modal{mTitle, flags};
  if (modal.IsOpen()) {
    UpdateContents(registry, dispatcher);

    ImGui::Spacing();

    if (Button(mAcceptButtonLabel, nullptr, IsCurrentInputValid(registry))) {
      OnAccept(dispatcher);
      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
      ImGui::CloseCurrentPopup();
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

}  // namespace Tactile