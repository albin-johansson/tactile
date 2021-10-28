#include "add_property_dialog.hpp"

#include <array>  // array

#include <tactile-base/tactile_error.hpp>

#include <imgui.h>

#include "core/components/property_context.hpp"
#include "core/systems/property_system.hpp"
#include "core/utils/buffer_utils.hpp"
#include "editor/events/property_events.hpp"
#include "editor/gui/widgets/alignment.hpp"
#include "editor/gui/widgets/common/button.hpp"
#include "editor/gui/widgets/common/modal.hpp"
#include "property_type_combo.hpp"

namespace Tactile {
namespace {

constexpr auto flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;

constinit std::array<char, 100> name_buffer{};
constinit PropertyType property_type = PropertyType::String;
constinit bool is_input_valid = false;

void ResetState()
{
  ZeroBuffer(name_buffer);
  property_type = PropertyType::String;
  is_input_valid = false;
}

}  // namespace

void UpdateAddPropertyDialog(const entt::registry& registry, entt::dispatcher& dispatcher)
{
  CenterNextWindowOnAppearance();
  if (auto modal = Modal{"Add property", flags}) {
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Name: ");

    ImGui::SameLine();

    if (ImGui::InputTextWithHint("##NameInput",
                                 "Unique property name...",
                                 name_buffer.data(),
                                 sizeof name_buffer))
    {
      const auto name = CreateStringFromBuffer(name_buffer);
      is_input_valid = !name.empty() && !Sys::HasPropertyWithName(registry, name);
    }

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Type: ");
    ImGui::SameLine();

    PropertyTypeCombo(property_type);

    ImGui::Spacing();
    if (Button("OK", nullptr, is_input_valid)) {
      dispatcher.enqueue<AddPropertyEvent>(CreateStringFromBuffer(name_buffer),
                                           property_type);
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

void OpenAddPropertyDialog()
{
  ResetState();
  ImGui::OpenPopup("Add property");
}

}  // namespace Tactile
