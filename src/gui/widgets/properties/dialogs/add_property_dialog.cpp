#include "add_property_dialog.hpp"

#include <imgui.h>

#include <array>  // array

#include "core/document.hpp"
#include "core/tactile_error.hpp"
#include "events/properties/add_property_event.hpp"
#include "gui/widgets/common/button_ex.hpp"
#include "gui/widgets/properties/dialogs/property_type_combo.hpp"
#include "utils/buffer_utils.hpp"

namespace Tactile {
namespace {

constinit std::array<char, 100> name_buffer{};
constinit int type_index = 0;
constinit bool is_input_valid = false;

void ResetState()
{
  type_index = 0;
  ZeroBuffer(name_buffer);
  is_input_valid = false;
}

}  // namespace

void UpdateAddPropertyDialog(const IDocument& document, entt::dispatcher& dispatcher)
{
  constexpr auto flags =
      ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;
  if (ImGui::BeginPopupModal("Add property", nullptr, flags))
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

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Type: ");

    ImGui::SameLine();
    PropertyTypeCombo(&type_index);

    ImGui::Spacing();
    if (ButtonEx("OK", nullptr, is_input_valid))
    {
      const auto type = GetPropertyTypeFromComboIndex(type_index);
      dispatcher.enqueue<AddPropertyEvent>(CreateStringFromBuffer(name_buffer),
                                           type);
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

void OpenAddPropertyDialog()
{
  ImGui::OpenPopup("Add property");
}

}  // namespace Tactile
