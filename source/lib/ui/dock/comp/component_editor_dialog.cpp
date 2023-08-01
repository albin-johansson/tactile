/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include "component_editor_dialog.hpp"

#include <utility>  // move

#include <imgui.h>

#include "common/util/vectors.hpp"
#include "core/attributes/attribute.hpp"
#include "core/containers/string.hpp"
#include "core/debug/assert.hpp"
#include "model/components/component_components.hpp"
#include "model/documents/document_components.hpp"
#include "model/documents/document_system.hpp"
#include "model/events/component_events.hpp"
#include "model/registry.hpp"
#include "model/services/language_service.hpp"
#include "model/services/service_locator.hpp"
#include "ui/constants.hpp"
#include "ui/dialog/dialog.hpp"
#include "ui/dock/comp/dialogs/new_comp_dialog.hpp"
#include "ui/dock/comp/dialogs/new_component_attribute_dialog.hpp"
#include "ui/dock/comp/dialogs/rename_component_attribute_dialog.hpp"
#include "ui/dock/comp/dialogs/rename_component_dialog.hpp"
#include "ui/style/icons.hpp"
#include "ui/widget/attribute_widgets.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile {
namespace {

constexpr const char* kComponentActionPopupId = "##ComponentActionPopup";

void _push_component_action_popup(ModelView model, ComponentEditorDialogState& state)
{
  const auto& strings = model.get_language_strings();

  if (const ui::Popup popup {kComponentActionPopupId}; popup.is_open()) {
    if (ImGui::MenuItem(strings.action.rename_component.c_str())) {
      model.enqueue<ShowRenameCompDialogEvent>(state.active_definition);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(strings.action.remove_component.c_str())) {
      model.enqueue<UndefComponentEvent>(state.active_definition);
      state.active_definition = kNullEntity;
    }
  }
}

void _push_component_selector_combo(ModelView model,
                                    const ComponentSet& component_set,
                                    ComponentEditorDialogState& state)
{
  const auto& registry = model.get_registry();

  if (state.active_definition == kNullEntity) {
    TACTILE_ASSERT(!component_set.definitions.empty());
    state.active_definition = component_set.definitions.front();
  }

  const auto& active_component_def = registry.get<Component>(state.active_definition);

  if (const ui::Combo combo {"##ComponentCombo", active_component_def.name.c_str()};
      combo.is_open()) {
    for (const auto definition_entity: component_set.definitions) {
      const auto& component = registry.get<Component>(definition_entity);

      if (ui::Selectable::property(component.name.c_str())) {
        state.active_definition = definition_entity;
      }
    }
  }
}

void _push_component_selector_row(ModelView model,
                                  const ComponentSet& component_set,
                                  ComponentEditorDialogState& state)
{
  const auto& strings = model.get_language_strings();

  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted(strings.misc.component.c_str());
  ImGui::SameLine();

  _push_component_selector_combo(model, component_set, state);

  ImGui::SameLine();
  if (ui::push_button(TAC_ICON_ADD, strings.tooltip.create_component.c_str())) {
    model.enqueue<ShowNewCompDialogEvent>();
  }

  ImGui::SameLine();
  if (ui::push_button(TAC_ICON_THREE_DOTS,
                      strings.tooltip.show_component_actions.c_str())) {
    ImGui::OpenPopup(kComponentActionPopupId);
  }

  _push_component_action_popup(model, state);
}

void _push_component_attribute_row_name_popup(ModelView model,
                                              const Entity definition_entity,
                                              const String& attribute_name)
{
  const auto& strings = model.get_language_strings();

  if (auto popup = ui::Popup::for_item("##ComponentAttributeNamePopup");
      popup.is_open()) {
    if (ImGui::MenuItem(strings.action.rename_attribute.c_str())) {
      model.enqueue<ShowRenameCompAttrDialogEvent>(definition_entity, attribute_name);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(strings.action.duplicate_attribute.c_str())) {
      model.enqueue<DuplicateComponentAttrEvent>(definition_entity, attribute_name);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(strings.action.remove_attribute.c_str())) {
      model.enqueue<RemoveComponentAttrEvent>(definition_entity, attribute_name);
    }
  }
}

void _push_component_attribute_row(ModelView model,
                                   const Entity definition_entity,
                                   const String& attribute_name,
                                   const Attribute& attribute)
{
  const auto& strings = model.get_language_strings();

  const ui::Scope scope {attribute_name.c_str()};

  ImGui::TableNextRow();
  ImGui::TableNextColumn();

  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted(attribute_name.c_str());

  _push_component_attribute_row_name_popup(model, definition_entity, attribute_name);

  ImGui::TableNextColumn();
  ImGui::SetNextItemWidth(-kMinFloat);

  if (const auto new_attribute_type =
          ui::push_attribute_type_combo(strings, attribute.get_type())) {
    model.enqueue<SetComponentAttrTypeEvent>(definition_entity,
                                             attribute_name,
                                             *new_attribute_type);
  }

  ImGui::TableNextColumn();

  if (auto updated_attribute = ui::push_attribute_input(strings, "##Value", attribute)) {
    model.enqueue<UpdateComponentEvent>(definition_entity,
                                        attribute_name,
                                        std::move(*updated_attribute));
  }
}

void _push_component_attribute_table(ModelView model,
                                     const Entity definition_entity,
                                     const Component& component)
{
  const auto& strings = model.get_language_strings();

  if (component.attributes.empty()) {
    ui::push_centered_label(strings.misc.empty_component.c_str());
  }
  else {
    const auto table_flags = ImGuiTableFlags_PadOuterX | ImGuiTableFlags_Resizable;
    if (const ui::Table table {"##ComponentAttributeTable", 3, table_flags};
        table.is_open()) {
      ImGui::TableSetupColumn(strings.misc.name.c_str(),
                              ImGuiTableColumnFlags_WidthStretch);
      ImGui::TableSetupColumn(strings.misc.type.c_str(),
                              ImGuiTableColumnFlags_WidthStretch);
      ImGui::TableSetupColumn(strings.misc.default_value.c_str(),
                              ImGuiTableColumnFlags_WidthStretch);

      ImGui::TableHeadersRow();

      for (const auto& [attr_name, attr_value]: component.attributes) {
        _push_component_attribute_row(model, definition_entity, attr_name, attr_value);
      }
    }
  }

  if (ui::push_centered_button(strings.action.create_attribute.c_str())) {
    model.enqueue<ShowNewCompAttrDialogEvent>(definition_entity);
  }
}

}  // namespace

void push_component_editor_dialog(ModelView model, ComponentEditorDialogState& state)
{
  const auto& registry = model.get_registry();
  const auto& strings = model.get_language_strings();

  ui::DialogOptions dialog_options {
      .title = strings.window.component_editor.c_str(),
      .close_label = strings.misc.close.c_str(),
      .flags = ui::UI_DIALOG_FLAG_INPUT_IS_VALID,
  };

  if (state.should_open) {
    dialog_options.flags |= ui::UI_DIALOG_FLAG_OPEN;
    state.should_open = false;
  }

  if (const ui::ScopedDialog dialog {dialog_options}; dialog.was_opened()) {
    const auto document_entity = sys::get_active_document(registry);
    const auto& document = registry.get<Document>(document_entity);
    const auto& component_set = registry.get<ComponentSet>(document.component_set);

    // Ensure that the active component entity hasn't been invalidated
    if (!contained_in(component_set.definitions, state.active_definition)) {
      state.active_definition = kNullEntity;
    }

    if (component_set.definitions.empty()) {
      ImGui::TextUnformatted(strings.misc.map_has_no_components.c_str());

      if (ui::push_centered_button(TAC_ICON_ADD,
                                   strings.tooltip.create_component.c_str())) {
        model.enqueue<ShowNewCompDialogEvent>();
      }
    }
    else {
      _push_component_selector_row(model, component_set, state);

      ImGui::Separator();

      if (state.active_definition != kNullEntity) {
        const auto& active_component_def =
            registry.get<Component>(state.active_definition);
        _push_component_attribute_table(model,
                                        state.active_definition,
                                        active_component_def);
      }
    }

    push_new_comp_dialog(model, state.new_comp_dialog);
    push_rename_comp_dialog(model, state.rename_comp_dialog);
    push_new_comp_attr_dialog(model, state.new_comp_attr_dialog);
    push_rename_comp_attr_dialog(model, state.rename_comp_attr_dialog);

    ImGui::Spacing();
    ImGui::Separator();
  }
}

}  // namespace tactile
