#include "rename_dialog.hpp"

#include <array>    // array
#include <cassert>  // assert
#include <utility>  // move

#include <imgui.h>

#include "core/utils/buffer_utils.hpp"
#include "editor/gui/widgets/alignment.hpp"
#include "editor/gui/widgets/common/button.hpp"
#include "editor/gui/widgets/common/modal.hpp"

namespace Tactile {
namespace {

constexpr auto gFlags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;

constinit std::array<char, 100> gNameBuffer{};
constinit bool gIsInputValid = false;

void ResetState()
{
  ZeroBuffer(gNameBuffer);
  gIsInputValid = false;
}

}  // namespace

// FIXME the rename dialog does not respond to user input
void UpdateRenameDialog(const NotNull<CStr> title,
                        const entt::registry& registry,
                        entt::dispatcher& dispatcher,
                        RenameDialogInputValidator validator,
                        RenameDialogResultCallback callback)
{
  assert(title);
  assert(validator);
  assert(callback);

  CenterNextWindowOnAppearance();
  if (auto modal = Modal{title, gFlags}) {
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Name: ");

    ImGui::SameLine();

    if (!ImGui::IsAnyItemActive()) {
      ImGui::SetKeyboardFocusHere();
    }

    if (ImGui::InputText("##NameInput", gNameBuffer.data(), sizeof gNameBuffer)) {
      const auto name = CreateStringFromBuffer(gNameBuffer);
      gIsInputValid = validator(registry, name);
    }

    ImGui::Spacing();
    if (Button("OK", nullptr, gIsInputValid)) {
      auto name = CreateStringFromBuffer(gNameBuffer);
      callback(dispatcher, std::move(name));
      ResetState();
      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
      ResetState();
      ImGui::CloseCurrentPopup();
    }
  }
}

void OpenRenameDialog(const NotNull<CStr> title, const std::string_view oldName)
{
  assert(title);
  CopyStringIntoBuffer(gNameBuffer, oldName);
  ImGui::OpenPopup(title);
}

}  // namespace Tactile
