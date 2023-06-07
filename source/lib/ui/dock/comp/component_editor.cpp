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

#include "component_editor.hpp"

#include <utility>  // move

#include <imgui.h>

#include "common/attribute.hpp"
#include "common/debug/assert.hpp"
#include "common/type/string.hpp"
#include "components/component.hpp"
#include "components/document.hpp"
#include "model/event/component_events.hpp"
#include "model/model.hpp"
#include "model/systems/document_system.hpp"
#include "model/systems/language_system.hpp"
#include "ui/constants.hpp"
#include "ui/dialog/dialog.hpp"
#include "ui/dock/comp/dialogs/add_component_attr_dialog.hpp"
#include "ui/dock/comp/dialogs/define_component_dialog.hpp"
#include "ui/dock/comp/dialogs/rename_component_attribute_dialog.hpp"
#include "ui/dock/comp/dialogs/rename_component_dialog.hpp"
#include "ui/style/icons.hpp"
#include "ui/widget/attribute_widgets.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile::ui {
namespace {

constexpr const char* kComponentActionPopupId = "##ComponentActionPopup";

void _push_component_action_popup(const Strings& strings,
                                  const Model& model,
                                  ComponentEditorDialogState& state,
                                  Dispatcher& dispatcher)
{
  if (const Popup popup {kComponentActionPopupId}; popup.is_open()) {
    const auto active_definition_entity = state.active_component_def.value();

    if (ImGui::MenuItem(strings.action.rename_component.c_str())) {
      const auto& active_definition =
          model.get<ComponentDefinition>(active_definition_entity);

      open_rename_component_dialog(active_definition_entity, active_definition.name);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(strings.action.remove_component.c_str())) {
      dispatcher.enqueue<UndefComponentEvent>(active_definition_entity);
      state.active_component_def.reset();
    }
  }
}

void _push_component_selector_combo(const Model& model,
                                    const ComponentSet& component_set,
                                    ComponentEditorDialogState& state)
{
  if (!state.active_component_def.has_value()) {
    TACTILE_ASSERT(!component_set.definitions.empty());
    state.active_component_def = component_set.definitions.front();
  }

  const auto& active_component_def =
      model.get<ComponentDefinition>(state.active_component_def.value());

  if (const Combo combo {"##ComponentCombo", active_component_def.name.c_str()};
      combo.is_open()) {
    for (const auto definition_entity: component_set.definitions) {
      const auto& component_def = model.get<ComponentDefinition>(definition_entity);

      if (Selectable::property(component_def.name.c_str())) {
        state.active_component_def = definition_entity;
      }
    }
  }
}

void _push_component_selector_row(const Strings& strings,
                                  const Model& model,
                                  const ComponentSet& component_set,
                                  ComponentEditorDialogState& state,
                                  Dispatcher& dispatcher)
{
  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted(strings.misc.component.c_str());
  ImGui::SameLine();

  _push_component_selector_combo(model, component_set, state);

  ImGui::SameLine();
  if (push_button(TAC_ICON_ADD, strings.tooltip.create_component.c_str())) {
    open_define_component_dialog();
  }

  ImGui::SameLine();
  if (push_button(TAC_ICON_THREE_DOTS, strings.tooltip.show_component_actions.c_str())) {
    ImGui::OpenPopup(kComponentActionPopupId);
  }

  _push_component_action_popup(strings, model, state, dispatcher);
}

void _push_component_attribute_row_name_popup(const Strings& strings,
                                              const Entity definition_entity,
                                              const String& attribute_name,
                                              Dispatcher& dispatcher)
{
  if (auto popup = Popup::for_item("##ComponentAttributeNamePopup"); popup.is_open()) {
    if (ImGui::MenuItem(strings.action.rename_attribute.c_str())) {
      open_rename_component_attribute_dialog(definition_entity, attribute_name);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(strings.action.duplicate_attribute.c_str())) {
      dispatcher.enqueue<DuplicateComponentAttrEvent>(definition_entity, attribute_name);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(strings.action.remove_attribute.c_str())) {
      dispatcher.enqueue<RemoveComponentAttrEvent>(definition_entity, attribute_name);
    }
  }
}

void _push_component_attribute_row(const Strings& strings,
                                   const Entity definition_entity,
                                   const String& attribute_name,
                                   const Attribute& attribute,
                                   Dispatcher& dispatcher)
{
  const Scope scope {attribute_name.c_str()};

  ImGui::TableNextRow();
  ImGui::TableNextColumn();

  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted(attribute_name.c_str());

  _push_component_attribute_row_name_popup(strings,
                                           definition_entity,
                                           attribute_name,
                                           dispatcher);

  ImGui::TableNextColumn();
  ImGui::SetNextItemWidth(-kMinFloat);

  if (const auto new_attribute_type =
          push_attribute_type_combo(strings, attribute.get_type())) {
    dispatcher.enqueue<SetComponentAttrTypeEvent>(definition_entity,
                                                  attribute_name,
                                                  *new_attribute_type);
  }

  ImGui::TableNextColumn();

  if (auto updated_attribute = push_attribute_input(strings, "##Value", attribute)) {
    dispatcher.enqueue<UpdateComponentEvent>(definition_entity,
                                             attribute_name,
                                             std::move(*updated_attribute));
  }
}

void _push_component_attribute_table(const Strings& strings,
                                     const Entity definition_entity,
                                     const ComponentDefinition& component_def,
                                     Dispatcher& dispatcher)
{
  if (component_def.attributes.empty()) {
    push_centered_label(strings.misc.empty_component.c_str());
  }
  else {
    const auto table_flags = ImGuiTableFlags_PadOuterX | ImGuiTableFlags_Resizable;
    if (const Table table {"##ComponentAttributeTable", 3, table_flags};
        table.is_open()) {
      ImGui::TableSetupColumn(strings.misc.name.c_str(),
                              ImGuiTableColumnFlags_WidthStretch);
      ImGui::TableSetupColumn(strings.misc.type.c_str(),
                              ImGuiTableColumnFlags_WidthStretch);
      ImGui::TableSetupColumn(strings.misc.default_value.c_str(),
                              ImGuiTableColumnFlags_WidthStretch);

      ImGui::TableHeadersRow();

      for (const auto& [attr_name, attr_value]: component_def.attributes) {
        _push_component_attribute_row(strings,
                                      definition_entity,
                                      attr_name,
                                      attr_value,
                                      dispatcher);
      }
    }
  }

  if (push_centered_button(strings.action.create_attribute.c_str())) {
    open_create_component_attribute_dialog(definition_entity);
  }
}

}  // namespace

void push_component_editor_dialog(const Model& model,
                                  ComponentEditorDialogState& state,
                                  Dispatcher& dispatcher)
{
  const auto& strings = sys::get_current_language_strings(model);

  DialogOptions dialog_options {
      .title = strings.window.component_editor.c_str(),
      .close_label = strings.misc.close.c_str(),
      .flags = UI_DIALOG_FLAG_INPUT_IS_VALID,
  };

  if (state.should_open) {
    dialog_options.flags |= UI_DIALOG_FLAG_OPEN;
    state.should_open = false;
  }

  if (const ScopedDialog dialog {dialog_options}; dialog.was_opened()) {
    const auto document_entity = sys::get_active_document(model);
    const auto& document = model.get<Document>(document_entity);
    const auto& component_set = model.get<ComponentSet>(document.component_set);

    // Ensure that the active component entity hasn't been invalidated
    if (state.active_component_def.has_value() &&
        !component_set.has_component(*state.active_component_def)) {
      state.active_component_def.reset();
    }

    if (component_set.definitions.empty()) {
      ImGui::TextUnformatted(strings.misc.map_has_no_components.c_str());

      if (push_centered_button(TAC_ICON_ADD, strings.tooltip.create_component.c_str())) {
        open_define_component_dialog();
      }
    }
    else {
      _push_component_selector_row(strings, model, component_set, state, dispatcher);

      ImGui::Separator();

      if (state.active_component_def.has_value()) {
        const auto& active_component_def =
            model.get<ComponentDefinition>(*state.active_component_def);
        _push_component_attribute_table(strings,
                                        *state.active_component_def,
                                        active_component_def,
                                        dispatcher);
      }
    }

    update_define_component_dialog(model, dispatcher);
    update_rename_component_dialog(model, dispatcher);
    update_create_component_attribute_dialog(model, dispatcher);
    update_rename_component_attribute_dialog(model, dispatcher);

    ImGui::Spacing();
    ImGui::Separator();
  }
}

}  // namespace tactile::ui
