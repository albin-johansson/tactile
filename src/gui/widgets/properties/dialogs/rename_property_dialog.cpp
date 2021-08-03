#include "rename_property_dialog.hpp"

#include <imgui.h>

#include <array>    // array
#include <utility>  // move

#include "aliases/maybe.hpp"
#include "core/document.hpp"
#include "core/events/properties/rename_property_event.hpp"
#include "gui/widgets/common/button_ex.hpp"
#include "utils/buffer_utils.hpp"

namespace Tactile {
namespace {

inline std::array<char, 100> name_buffer{};
inline Maybe<std::string> old_name;
inline bool is_input_valid = false;

void ResetState()
{
  ZeroBuffer(name_buffer);
  old_name.reset();
  is_input_valid = false;
}

}  // namespace

void UpdateRenamePropertyDialog(const IDocument& document,
                                entt::dispatcher& dispatcher)
{
  constexpr auto flags =
      ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;
  if (ImGui::BeginPopupModal("Rename property", nullptr, flags))
  {
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Name: ");

    ImGui::SameLine();
    if (ImGui::InputText("##NameInput", name_buffer.data(), sizeof name_buffer))
    {
      const auto name = CreateStringFromBuffer(name_buffer);
      const auto& properties = document.GetProperties();
      is_input_valid = !name.empty() && !properties.contains(name);
    }

    ImGui::Spacing();
    if (ButtonEx("OK", nullptr, is_input_valid))
    {
      auto name = CreateStringFromBuffer(name_buffer);
      dispatcher.enqueue<RenamePropertyEvent>(old_name.value(), std::move(name));
      ResetState();
      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();
    if (ImGui::Button("Cancel"))
    {
      ResetState();
      ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
  }
}

void OpenRenamePropertyDialog(std::string name)
{
  old_name = std::move(name);
  CopyStringIntoBuffer(name_buffer, old_name.value());
  ImGui::OpenPopup("Rename property");
}

}  // namespace Tactile
