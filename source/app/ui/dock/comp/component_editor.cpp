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

#include <utility>  // move

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>

#include "core/attribute.hpp"
#include "core/component/component_index.hpp"
#include "core/debug/assert.hpp"
#include "core/type/maybe.hpp"
#include "core/type/string.hpp"
#include "core/type/uuid.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/event/component_events.hpp"
#include "model/model.hpp"
#include "ui/constants.hpp"
#include "ui/dialog/dialog.hpp"
#include "ui/dock/comp/dialogs/add_component_attr_dialog.hpp"
#include "ui/dock/comp/dialogs/define_component_dialog.hpp"
#include "ui/dock/comp/dialogs/rename_component_attribute_dialog.hpp"
#include "ui/dock/comp/dialogs/rename_component_dialog.hpp"
#include "ui/dock/property/dialogs/property_type_combo.hpp"
#include "ui/style/icons.hpp"
#include "ui/widget/input_widgets.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile::ui {
namespace {

inline Maybe<UUID> dialog_active_component;
inline constinit bool open_dialog = false;

void show_component_combo_popup(const Document& document, entt::dispatcher& dispatcher)
{
  if (const Popup popup {"##ComponentEditorPopup"}; popup.is_open()) {
    const auto& lang = get_current_language();

    if (ImGui::MenuItem(lang.action.rename_component.c_str())) {
      const auto component_id = dialog_active_component.value();
      const auto* component_index = document.view_component_index();
      const auto& component_name = component_index->at(component_id).name();
      open_rename_component_dialog(component_id, component_name);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(lang.action.remove_component.c_str())) {
      dispatcher.enqueue<UndefComponentEvent>(dialog_active_component.value());
      dialog_active_component.reset();
    }
  }
}

void show_component_attribute(const UUID& component_id,
                              const String& attribute_name,
                              const Attribute& value,
                              entt::dispatcher& dispatcher)
{
  const auto& lang = get_current_language();

  const Scope scope {attribute_name.c_str()};

  ImGui::TableNextRow();
  ImGui::TableNextColumn();

  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted(attribute_name.c_str());

  if (auto popup = Popup::for_item("##ComponentAttributeNameContext"); popup.is_open()) {
    if (ImGui::MenuItem(lang.action.rename_attribute.c_str())) {
      open_rename_component_attribute_dialog(dialog_active_component.value(),
                                             attribute_name);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(lang.action.duplicate_attribute.c_str())) {
      dispatcher.enqueue<DuplicateComponentAttrEvent>(component_id, attribute_name);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(lang.action.remove_attribute.c_str())) {
      dispatcher.enqueue<RemoveComponentAttrEvent>(component_id, attribute_name);
    }
  }

  ImGui::TableNextColumn();
  ImGui::SetNextItemWidth(-min_float);

  const auto type = value.type();
  AttributeType new_type = type;
  show_property_type_combo(type, new_type);
  if (new_type != type) {
    dispatcher.enqueue<SetComponentAttrTypeEvent>(component_id, attribute_name, new_type);
  }

  ImGui::TableNextColumn();

  if (auto updated = input_attribute("##DefaultValue", value)) {
    dispatcher.enqueue<UpdateComponentEvent>(component_id,
                                             attribute_name,
                                             std::move(*updated));
  }
}

void show_component_attributes(const ComponentDefinition& definition,
                               entt::dispatcher& dispatcher)
{
  const auto& lang = get_current_language();

  if (definition.empty()) {
    ui_centered_label(lang.misc.empty_component.c_str());
  }
  else {
    constexpr auto table_flags = ImGuiTableFlags_PadOuterX | ImGuiTableFlags_Resizable;
    if (const Table table {"##ComponentAttributeTable", 3, table_flags};
        table.is_open()) {
      ImGui::TableSetupColumn(lang.misc.name.c_str(), ImGuiTableColumnFlags_WidthStretch);
      ImGui::TableSetupColumn(lang.misc.type.c_str(), ImGuiTableColumnFlags_WidthStretch);
      ImGui::TableSetupColumn(lang.misc.default_value.c_str(),
                              ImGuiTableColumnFlags_WidthStretch);
      ImGui::TableHeadersRow();

      for (const auto& [name, attr]: definition) {
        show_component_attribute(definition.get_uuid(), name, attr, dispatcher);
      }
    }
  }

  if (ui_centered_button(lang.action.create_attribute.c_str())) {
    open_create_component_attribute_dialog(dialog_active_component.value());
  }
}

}  // namespace

void open_component_editor_dialog(const DocumentModel& model)
{
  const auto* component_index = model.require_active_document().view_component_index();
  TACTILE_ASSERT(component_index != nullptr);

  dialog_active_component =
      !component_index->empty() ? Maybe<UUID> {component_index->begin()->first} : nothing;
  open_dialog = true;
}

void update_component_editor_dialog(const DocumentModel& model,
                                    entt::dispatcher& dispatcher)
{
  const auto& lang = get_current_language();

  DialogOptions options {
      .title = lang.window.component_editor.c_str(),
      .close_label = lang.misc.close.c_str(),
      .flags = UI_DIALOG_FLAG_INPUT_IS_VALID,
  };

  if (open_dialog) {
    options.flags |= UI_DIALOG_FLAG_OPEN;
    open_dialog = false;
  }

  if (const ScopedDialog dialog {options}; dialog.was_opened()) {
    const auto& document = model.require_active_document();
    const auto* component_index = document.view_component_index();
    TACTILE_ASSERT(component_index != nullptr);

    // Ensure that the active component ID hasn't been invalidated
    if (dialog_active_component && !component_index->contains(*dialog_active_component)) {
      dialog_active_component.reset();
    }

    if (component_index->empty()) {
      ImGui::TextUnformatted(lang.misc.map_has_no_components.c_str());

      if (ui_centered_button(TAC_ICON_ADD, lang.tooltip.create_component.c_str())) {
        open_define_component_dialog();
      }
    }
    else {
      ImGui::AlignTextToFramePadding();
      ImGui::TextUnformatted(lang.misc.component.c_str());
      ImGui::SameLine();

      if (!dialog_active_component) {
        dialog_active_component = component_index->begin()->first;
      }

      const auto& definition = component_index->at(dialog_active_component.value());
      const auto& name = definition.name();
      if (const Combo combo {"##ComponentEditorCombo", name.c_str()}; combo.is_open()) {
        for (const auto& [componentId, component]: *component_index) {
          if (Selectable::Property(component.name().c_str())) {
            dialog_active_component = component.get_uuid();
          }
        }
      }

      ImGui::SameLine();

      if (ui_button(TAC_ICON_ADD, lang.tooltip.create_component.c_str())) {
        open_define_component_dialog();
      }

      ImGui::SameLine();

      if (ui_button(TAC_ICON_THREE_DOTS, lang.tooltip.show_component_actions.c_str())) {
        ImGui::OpenPopup("##ComponentEditorPopup");
      }

      show_component_combo_popup(document, dispatcher);

      ImGui::Separator();
    }

    if (dialog_active_component) {
      const auto& definition = component_index->at(*dialog_active_component);
      show_component_attributes(definition, dispatcher);
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
