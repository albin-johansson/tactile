#include "rename_dialog.hpp"

#include <imgui.h>

#include <array>    // array
#include <cassert>  // assert
#include <utility>  // move

#include "core/utils/buffer_utils.hpp"
#include "gui/widgets/alignment.hpp"
#include "gui/widgets/common/button.hpp"
#include "gui/widgets/common/modal.hpp"

namespace Tactile {
namespace {

constexpr auto flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;

constinit std::array<char, 100> name_buffer{};
constinit bool is_input_valid = false;

void ResetState()
{
  ZeroBuffer(name_buffer);
  is_input_valid = false;
}

}  // namespace

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
  if (auto modal = Modal{title, flags}) {
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Name: ");

    ImGui::SameLine();

    if (!ImGui::IsAnyItemActive()) {
      ImGui::SetKeyboardFocusHere();
    }

    if (ImGui::InputText("##NameInput", name_buffer.data(), sizeof name_buffer)) {
      const auto name = CreateStringFromBuffer(name_buffer);
      is_input_valid = validator(registry, name);
    }

    ImGui::Spacing();
    if (Button("OK", nullptr, is_input_valid)) {
      auto name = CreateStringFromBuffer(name_buffer);
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
  CopyStringIntoBuffer(name_buffer, oldName);
  ImGui::OpenPopup(title);
}

}  // namespace Tactile
