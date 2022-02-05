#include "component_editor.hpp"

#include <string>   // string
#include <utility>  // move

#include <centurion.hpp>
#include <imgui.h>

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

namespace tactile {

ComponentEditor::ComponentEditor() : ADialog{"Component Editor"}
{
  SetAcceptButtonLabel(nullptr);
  SetCloseButtonLabel("Close");
}

void ComponentEditor::Open(const Model& model)
{
  const auto& registry = model.GetActiveRegistryRef();
  mActiveComponent = sys::get_first_available_component_def(registry);
  Show();
}

void ComponentEditor::UpdateContents(const Model& model, entt::dispatcher& dispatcher)
{
  const auto& registry = model.GetActiveRegistryRef();

  /* Ensure that the active component ID hasn't been invalidated */
  if (mActiveComponent && !sys::is_valid_component(registry, *mActiveComponent)) {
    mActiveComponent.reset();
  }

  if (registry.storage<comp::component_def>().empty()) {
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
      const auto entity = registry.view<comp::component_def>().front();
      mActiveComponent = registry.get<comp::component_def>(entity).id;
    }

    const auto& name = sys::get_component_def_name(registry, mActiveComponent.value());
    if (scoped::Combo combo{"##ComponentEditorCombo", name.c_str()}; combo.IsOpen()) {
      for (auto&& [entity, component] : registry.view<comp::component_def>().each()) {
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

    ShowComponentComboPopup(registry, dispatcher);

    ImGui::Separator();
  }

  if (mActiveComponent) {
    ShowComponentAttributes(registry, dispatcher, *mActiveComponent);
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
  if (scoped::Popup popup{"##ComponentEditorPopup"}; popup.IsOpen()) {
    if (ImGui::MenuItem(TAC_ICON_EDIT " Rename Component")) {
      const auto id = mActiveComponent.value();
      mRenameComponentDialog.Open(sys::get_component_def_name(registry, id), id);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_REMOVE " Remove Component")) {
      dispatcher.enqueue<remove_component_def_event>(mActiveComponent.value());
      mActiveComponent.reset();
    }
  }
}

void ComponentEditor::ShowComponentAttributes(const entt::registry& registry,
                                              entt::dispatcher& dispatcher,
                                              const component_id id)
{
  const auto& [defEntity, def] = sys::get_component_def(registry, id);

  if (def.attributes.empty()) {
    CenteredText("There are no attributes defined for the current component.");
  }
  else {
    constexpr auto table_flags = ImGuiTableFlags_PadOuterX | ImGuiTableFlags_Resizable;
    if (scoped::Table table{"##ComponentAttributeTable", 3, table_flags};
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
                                             const component_id id,
                                             const std::string& name,
                                             const attribute_value& value)
{
  const scoped::ID scope{name.c_str()};

  ImGui::TableNextRow();
  ImGui::TableNextColumn();

  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted(name.c_str());

  if (auto popup = scoped::Popup::ForItem("##ComponentAttributeNameContext");
      popup.IsOpen()) {
    if (ImGui::MenuItem(TAC_ICON_EDIT " Rename Attribute")) {
      mRenameComponentAttributeDialog.Open(name, mActiveComponent.value());
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_DUPLICATE " Duplicate Attribute")) {
      dispatcher.enqueue<duplicate_component_attr_event>(id, name);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_REMOVE " Remove Attribute")) {
      dispatcher.enqueue<remove_component_attr_event>(id, name);
    }
  }

  ImGui::TableNextColumn();
  ImGui::SetNextItemWidth(-min_float);

  const auto type = value.type();
  attribute_type newType = type;
  PropertyTypeCombo(type, newType);
  if (newType != type) {
    dispatcher.enqueue<set_component_attr_type_event>(id, name, newType);
  }

  ImGui::TableNextColumn();

  if (auto updated = Input("##DefaultValue", value)) {
    dispatcher.enqueue<update_component_def_attr_event>(id, name, std::move(*updated));
  }
}

}  // namespace tactile
