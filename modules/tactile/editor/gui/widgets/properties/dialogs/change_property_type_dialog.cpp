#include "change_property_type_dialog.hpp"

#include <utility>  // move

#include <tactile_def.hpp>
#include <tactile_stdlib.hpp>

#include <imgui.h>

#include "editor/events/property_events.hpp"
#include "editor/gui/widgets/alignment.hpp"
#include "editor/gui/widgets/common/button.hpp"
#include "editor/gui/widgets/common/modal.hpp"
#include "property_type_combo.hpp"

namespace Tactile {
namespace {

constexpr auto gFlags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;

constinit PropertyType gCurrentType = PropertyType::String;
inline Maybe<std::string> gPropertyName;
inline Maybe<PropertyType> gPreviousType;

void ResetState()
{
  gCurrentType = PropertyType::String;
  gPropertyName.reset();
  gPreviousType.reset();
}

}  // namespace

void UpdateChangePropertyTypeDialog(entt::dispatcher& dispatcher)
{
  CenterNextWindowOnAppearance();
  if (auto modal = Modal{"Change property type", gFlags}) {
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Type: ");

    ImGui::SameLine();
    PropertyTypeCombo(gPreviousType.value(), gCurrentType);

    ImGui::Spacing();

    if (Button("OK", nullptr, gCurrentType != gPreviousType.value())) {
      dispatcher.enqueue<ChangePropertyTypeEvent>(gPropertyName.value(), gCurrentType);

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

void OpenChangePropertyTypeDialog(std::string name, const PropertyType type)
{
  gPropertyName = std::move(name);
  gCurrentType = type;
  gPreviousType = type;
  ImGui::OpenPopup("Change property type");
}

}  // namespace Tactile
