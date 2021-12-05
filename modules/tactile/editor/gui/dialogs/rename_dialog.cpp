#include "rename_dialog.hpp"

#include <utility>  // move

#include <imgui.h>

#include "core/utils/buffer_utils.hpp"
#include "editor/gui/alignment.hpp"
#include "editor/gui/common/button.hpp"
#include "editor/gui/scoped.hpp"

namespace Tactile {

ARenameDialog::ARenameDialog(const CStr title) : mTitle{title} {}

void ARenameDialog::Update(const entt::registry& registry, entt::dispatcher& dispatcher)
{
  if (mShow) {
    ImGui::OpenPopup(mTitle);
    mShow = false;
  }

  constexpr auto flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;

  CenterNextWindowOnAppearance();
  if (Scoped::Modal modal{mTitle, flags}; modal.IsOpen()) {
    if (ImGui::InputText("##NameInput", mNameBuffer.data(), sizeof mNameBuffer)) {
      const auto name = CreateStringViewFromBuffer(mNameBuffer);
      mIsInputValid = IsInputValid(registry, name);
    }

    ImGui::SetItemDefaultFocus();

    ImGui::Spacing();
    if (Button("Rename", nullptr, mIsInputValid)) {
      OnAccept(dispatcher, CreateStringFromBuffer(mNameBuffer));
      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
      ImGui::CloseCurrentPopup();
    }
  }
}

void ARenameDialog::Show(std::string oldName)
{
  mOldName = std::move(oldName);
  mShow = true;
  CopyStringIntoBuffer(mNameBuffer, mOldName);
}

auto ARenameDialog::GetPreviousName() const -> const std::string&
{
  return mOldName;
}

}  // namespace Tactile
