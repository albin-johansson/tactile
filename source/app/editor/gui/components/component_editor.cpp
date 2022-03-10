/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "component_editor.hpp"

#include <string>   // string
#include <utility>  // move

#include <imgui.h>

#include "core/components/attributes.hpp"
#include "core/systems/component_system.hpp"
#include "dialogs/create_component_attribute_dialog.hpp"
#include "dialogs/create_component_dialog.hpp"
#include "dialogs/rename_component_attribute_dialog.hpp"
#include "dialogs/rename_component_dialog.hpp"
#include "editor/events/component_events.hpp"
#include "editor/gui/common/button.hpp"
#include "editor/gui/common/centered_text.hpp"
#include "editor/gui/common/input_widgets.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/properties/dialogs/property_type_combo.hpp"
#include "editor/gui/scoped.hpp"
#include "editor/model.hpp"

namespace tactile {

struct ComponentEditor::component_editor_data final
{
  create_component_dialog create_component;
  create_component_attribute_dialog create_component_attr;
  rename_component_dialog rename_component;
  rename_component_attribute_dialog rename_component_attr;
  Maybe<component_id> active_component;
};

ComponentEditor::ComponentEditor()
    : dialog_base{"Component Editor"}
    , mData{std::make_unique<component_editor_data>()}
{
  set_accept_button_label(nullptr);
  set_close_button_label("Close");
}

ComponentEditor::~ComponentEditor() noexcept = default;

void ComponentEditor::show(const DocumentModel& model)
{
  const auto& registry = model.get_active_registry();
  mData->active_component = sys::get_first_available_component_def(registry);
  make_visible();
}

void ComponentEditor::on_update(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  const auto& registry = model.get_active_registry();
  auto& data = *mData;

  /* Ensure that the active component ID hasn't been invalidated */
  if (data.active_component &&
      !sys::is_valid_component(registry, *data.active_component)) {
    data.active_component.reset();
  }

  if (registry.storage<comp::ComponentDef>().empty()) {
    ImGui::TextUnformatted("There are no available components for the current map.");

    if (centered_button(TAC_ICON_ADD, "Create Component")) {
      data.create_component.show();
    }
  }
  else {
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Component:");
    ImGui::SameLine();

    if (!data.active_component) {
      const auto entity = registry.view<comp::ComponentDef>().front();
      data.active_component = registry.get<comp::ComponentDef>(entity).id;
    }

    const auto& name =
        sys::get_component_def_name(registry, data.active_component.value());
    if (scoped::Combo combo{"##ComponentEditorCombo", name.c_str()}; combo.is_open()) {
      for (auto&& [entity, component] : registry.view<comp::ComponentDef>().each()) {
        if (ImGui::Selectable(component.name.c_str())) {
          data.active_component = component.id;
        }
      }
    }

    ImGui::SameLine();

    if (button(TAC_ICON_ADD, "Create Component")) {
      data.create_component.show();
    }

    ImGui::SameLine();

    if (button(TAC_ICON_THREE_DOTS, "Show Component Actions")) {
      ImGui::OpenPopup("##ComponentEditorPopup");
    }

    show_component_combo_popup(registry, dispatcher);

    ImGui::Separator();
  }

  if (data.active_component) {
    show_component_attributes(registry, dispatcher, *data.active_component);
  }

  data.create_component.update(model, dispatcher);
  data.create_component_attr.update(model, dispatcher);
  data.rename_component.update(model, dispatcher);
  data.rename_component_attr.update(model, dispatcher);

  ImGui::Spacing();
  ImGui::Separator();
}

void ComponentEditor::show_component_combo_popup(const entt::registry& registry,
                                                 entt::dispatcher& dispatcher)
{
  auto& data = *mData;
  if (scoped::Popup popup{"##ComponentEditorPopup"}; popup.is_open()) {
    if (ImGui::MenuItem(TAC_ICON_EDIT " Rename Component")) {
      const auto id = data.active_component.value();
      data.rename_component.show(sys::get_component_def_name(registry, id), id);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_REMOVE " Remove Component")) {
      dispatcher.enqueue<RemoveComponentDefEvent>(data.active_component.value());
      data.active_component.reset();
    }
  }
}

void ComponentEditor::show_component_attributes(const entt::registry& registry,
                                                entt::dispatcher& dispatcher,
                                                component_id id)
{
  auto& data = *mData;
  const auto& [defEntity, def] = sys::get_component_def(registry, id);

  if (def.attributes.empty()) {
    centered_text("There are no attributes defined for the current component.");
  }
  else {
    constexpr auto table_flags = ImGuiTableFlags_PadOuterX | ImGuiTableFlags_Resizable;
    if (scoped::Table table{"##ComponentAttributeTable", 3, table_flags};
        table.is_open()) {
      ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
      ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthStretch);
      ImGui::TableSetupColumn("Default", ImGuiTableColumnFlags_WidthStretch);
      ImGui::TableHeadersRow();

      for (const auto& [name, attr] : def.attributes) {
        show_component_attribute(dispatcher, id, name, attr);
      }
    }
  }

  if (centered_button("Create Attribute")) {
    data.create_component_attr.show(*data.active_component);
  }
}

void ComponentEditor::show_component_attribute(entt::dispatcher& dispatcher,
                                               component_id id,
                                               const std::string& name,
                                               const Attribute& value)
{
  auto& data = *mData;
  const scoped::Id scope{name.c_str()};

  ImGui::TableNextRow();
  ImGui::TableNextColumn();

  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted(name.c_str());

  if (auto popup = scoped::Popup::for_item("##ComponentAttributeNameContext");
      popup.is_open()) {
    if (ImGui::MenuItem(TAC_ICON_EDIT " Rename Attribute")) {
      data.rename_component_attr.show(name, data.active_component.value());
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_DUPLICATE " Duplicate Attribute")) {
      dispatcher.enqueue<DuplicateComponentAttrEvent>(id, name);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(TAC_ICON_REMOVE " Remove Attribute")) {
      dispatcher.enqueue<RemoveComponentAttrEvent>(id, name);
    }
  }

  ImGui::TableNextColumn();
  ImGui::SetNextItemWidth(-min_float);

  const auto type = value.type();
  AttributeType newType = type;
  PropertyTypeCombo(type, newType);
  if (newType != type) {
    dispatcher.enqueue<SetComponentAttrTypeEvent>(id, name, newType);
  }

  ImGui::TableNextColumn();

  if (auto updated = input_attribute("##DefaultValue", value)) {
    dispatcher.enqueue<UpdateComponentDefAttrEvent>(id, name, std::move(*updated));
  }
}

}  // namespace tactile
