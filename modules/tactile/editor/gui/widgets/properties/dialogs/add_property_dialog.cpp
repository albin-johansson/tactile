#include "add_property_dialog.hpp"

#include <array>  // array

#include <tactile_stdlib.hpp>

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

constexpr auto gFlags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;

constinit std::array<char, 100> gNameBuffer{};
constinit PropertyType gPropertyType = PropertyType::String;
constinit bool gIsInputValid = false;

void ResetState()
{
  ZeroBuffer(gNameBuffer);
  gPropertyType = PropertyType::String;
  gIsInputValid = false;
}

}  // namespace

void UpdateAddPropertyDialog(const entt::registry& registry, entt::dispatcher& dispatcher)
{
  CenterNextWindowOnAppearance();
  if (auto modal = Modal{"Add property", gFlags}) {
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Name: ");

    ImGui::SameLine();

    if (ImGui::InputTextWithHint("##NameInput",
                                 "Unique property name...",
                                 gNameBuffer.data(),
                                 sizeof gNameBuffer))
    {
      const auto name = CreateStringFromBuffer(gNameBuffer);
      const auto& context = Sys::GetCurrentContext(registry);
      gIsInputValid = !name.empty() && !Sys::HasPropertyWithName(registry, context, name);
    }

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Type: ");
    ImGui::SameLine();

    PropertyTypeCombo(gPropertyType);

    ImGui::Spacing();
    if (Button("OK", nullptr, gIsInputValid)) {
      dispatcher.enqueue<AddPropertyEvent>(CreateStringFromBuffer(gNameBuffer),
                                           gPropertyType);
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
