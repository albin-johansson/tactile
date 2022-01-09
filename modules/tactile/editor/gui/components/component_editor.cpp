#include "component_editor.hpp"

#define NOMINMAX

#include <limits>   // numeric_limits
#include <string>   // string
#include <utility>  // move

#include <centurion.hpp>
#include <imgui.h>

#include "assert.hpp"
#include "core/components/component.hpp"
#include "core/systems/component_system.hpp"
#include "editor/events/component_events.hpp"
#include "editor/gui/alignment.hpp"
#include "editor/gui/common/button.hpp"
#include "editor/gui/common/centered_button.hpp"
#include "editor/gui/common/centered_text.hpp"
#include "editor/gui/common/input_widgets.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/properties/dialogs/property_type_combo.hpp"
#include "editor/gui/scoped.hpp"
#include "editor/model.hpp"

namespace Tactile {

ComponentEditor::ComponentEditor() : ADialog{"Component Editor"}
{
  SetAcceptButtonLabel(nullptr);
  SetCloseButtonLabel("Close");
}

void ComponentEditor::Open(const Model& model)
{
  const auto* registry = model.GetActiveRegistry();
  TACTILE_ASSERT(registry);

  mActiveComponent = Sys::GetFirstAvailableComponentDef(*registry);
  Show();
}

void ComponentEditor::UpdateContents(const Model& model, entt::dispatcher& dispatcher)
{
  const auto* registry = model.GetActiveRegistry();
  TACTILE_ASSERT(registry);

  /* Ensure that the active component ID hasn't been invalidated */
  if (mActiveComponent && !Sys::IsComponentValid(*registry, *mActiveComponent)) {
    mActiveComponent.reset();
  }

  if (registry->storage<ComponentDef>().empty()) {
    ImGui::TextUnformatted("There are no available components for the current map.");

    if (CenteredButton(TAC_ICON_ADD, "Create Component")) {
      mCreateComponentDialog.Open();
    }
  }
  else {
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Component:");
    ImGui::SameLine();

    if (!mActiveComponent) {
      const auto entity = registry->view<ComponentDef>().front();
      mActiveComponent = registry->get<ComponentDef>(entity).id;
    }

    const auto& name = Sys::GetComponentDefName(*registry, mActiveComponent.value());
    if (Scoped::Combo combo{"##ComponentEditorCombo", name.c_str()}; combo.IsOpen()) {
      for (auto&& [entity, component] : registry->view<ComponentDef>().each()) {
        if (ImGui::Selectable(component.name.c_str())) {
          mActiveComponent = component.id;
        }
      }
    }

    ImGui::SameLine();

    if (Button(TAC_ICON_ADD, "Create Component")) {
      mCreateComponentDialog.Open();
    }

    ImGui::SameLine();

    if (Button(TAC_ICON_THREE_DOTS, "Show Component Actions")) {
      ImGui::OpenPopup("##ComponentEditorPopup");
    }

    ShowComponentComboPopup(*registry, dispatcher);

    ImGui::Separator();
  }

  if (mActiveComponent) {
    ShowComponentAttributes(*registry, dispatcher, *mActiveComponent);
  }

  mCreateComponentDialog.Update(model, dispatcher);
  mCreateComponentAttributeDialog.Update(model, dispatcher);
  mRenameComponentDialog.Update(model, dispatcher);
  mRenameComponentAttributeDialog.Update(model, dispatcher);

  ImGui::Spacing();
  ImGui::Separator();
}

void ComponentEditor::ShowComponentComboPopup(const entt::registry& registry,
                                              entt::dispatcher& dispatcher)
{
  if (Scoped::Popup popup{"##ComponentEditorPopup"}; popup.IsOpen()) {
    if (ImGui::MenuItem(TAC_ICON_EDIT " Rename Component")) {
      const auto id = mActiveComponent.value();
      mRenameComponentDialog.Open(Sys::GetComponentDefName(registry, id), id);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_REMOVE " Remove Component")) {
      dispatcher.enqueue<RemoveComponentDefEvent>(mActiveComponent.value());
      mActiveComponent.reset();
    }
  }
}

void ComponentEditor::ShowComponentAttributes(const entt::registry& registry,
                                              entt::dispatcher& dispatcher,
                                              const ComponentID id)
{
  const auto& [defEntity, def] = Sys::GetComponentDef(registry, id);

  if (def.attributes.empty()) {
    CenteredText("There are no attributes defined for the current component.");
  }
  else {
    constexpr auto table_flags = ImGuiTableFlags_PadOuterX | ImGuiTableFlags_Resizable;
    if (Scoped::Table table{"##ComponentAttributeTable", 3, table_flags};
        table.IsOpen()) {
      ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
      ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthStretch);
      ImGui::TableSetupColumn("Default", ImGuiTableColumnFlags_WidthStretch);
      ImGui::TableHeadersRow();

      for (const auto& [name, attr] : def.attributes) {
        ShowComponentAttribute(dispatcher, id, name, attr);
      }
    }
  }

  if (CenteredButton("Create Attribute")) {
    mCreateComponentAttributeDialog.Open(*mActiveComponent);
  }
}

void ComponentEditor::ShowComponentAttribute(entt::dispatcher& dispatcher,
                                             const ComponentID id,
                                             const std::string& name,
                                             const PropertyValue& value)
{
  const Scoped::ID scope{name.c_str()};

  ImGui::TableNextRow();
  ImGui::TableNextColumn();

  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted(name.c_str());

  if (auto popup = Scoped::Popup::ForItem("##ComponentAttributeNameContext");
      popup.IsOpen()) {
    if (ImGui::MenuItem(TAC_ICON_EDIT " Rename Attribute")) {
      mRenameComponentAttributeDialog.Open(name, mActiveComponent.value());
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_DUPLICATE " Duplicate Attribute")) {
      dispatcher.enqueue<DuplicateComponentAttributeEvent>(id, name);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_REMOVE " Remove Attribute")) {
      dispatcher.enqueue<RemoveComponentAttributeEvent>(id, name);
    }
  }

  ImGui::TableNextColumn();
  ImGui::SetNextItemWidth(-std::numeric_limits<float>::min());

  const auto type = value.GetType().value();
  PropertyType newType = type;
  PropertyTypeCombo(type, newType);
  if (newType != type) {
    dispatcher.enqueue<SetComponentAttributeTypeEvent>(id, name, newType);
  }

  ImGui::TableNextColumn();

  if (auto updated = Input("##DefaultValue", value)) {
    dispatcher.enqueue<UpdateComponentDefAttributeEvent>(id, name, std::move(*updated));
  }
}

}  // namespace Tactile
