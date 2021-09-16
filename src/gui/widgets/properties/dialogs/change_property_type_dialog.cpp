#include "change_property_type_dialog.hpp"

#include <imgui.h>

#include <utility>  // move

#include "aliases/maybe.hpp"
#include "core/tactile_error.hpp"
#include "events/property_events.hpp"
#include "gui/widgets/alignment.hpp"
#include "gui/widgets/properties/dialogs/property_type_combo.hpp"

namespace Tactile {
namespace {

constexpr auto flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;

constinit int type_index = 0;
inline Maybe<std::string> property_name;

void ResetState()
{
  type_index = 0;
  property_name.reset();
}

}  // namespace

void UpdateChangePropertyTypeDialog(const entt::registry& registry,
                                    entt::dispatcher& dispatcher)
{
  CenterNextWindowOnAppearance();
  if (ImGui::BeginPopupModal("Change property type", nullptr, flags)) {
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Type: ");

    ImGui::SameLine();
    PropertyTypeCombo(&type_index);

    ImGui::Spacing();
    if (ImGui::Button("OK")) {
      const auto type = GetPropertyTypeFromComboIndex(type_index);
      dispatcher.enqueue<ChangePropertyTypeEvent>(property_name.value(), type);

      ResetState();
      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
      ResetState();
      ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
  }
}

void OpenChangePropertyTypeDialog(std::string name)
{
  property_name = std::move(name);
  ImGui::OpenPopup("Change property type");
}

}  // namespace Tactile
