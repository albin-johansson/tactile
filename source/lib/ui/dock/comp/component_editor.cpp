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

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>

#include "common/debug/assert.hpp"
#include "common/type/uuid.hpp"
#include "core/attribute.hpp"
#include "core/component/component_index.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/event/component_events.hpp"
#include "model/model.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/string.hpp"
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

inline constexpr const char* kComponentActionPopupId = "##ComponentActionPopup";

struct ComponentEditorState final {
  Maybe<UUID> active_component_id;
  bool open_dialog {};
};

inline ComponentEditorState gEditorState;

void ui_component_action_popup(const Strings& lang,
                               const ComponentIndex* component_index,
                               entt::dispatcher& dispatcher)
{
  if (const Popup popup {kComponentActionPopupId}; popup.is_open()) {
    if (ImGui::MenuItem(lang.action.rename_component.c_str())) {
      const auto active_component_id = gEditorState.active_component_id.value();
      const auto& active_component_def = component_index->get_comp(active_component_id);
      open_rename_component_dialog(active_component_id, active_component_def.get_name());
    }

    ImGui::Separator();

    if (ImGui::MenuItem(lang.action.remove_component.c_str())) {
      dispatcher.enqueue<UndefComponentEvent>(gEditorState.active_component_id.value());
      gEditorState.active_component_id.reset();
    }
  }
}

void ui_component_combo(const ComponentIndex* component_index)
{
  if (!gEditorState.active_component_id) {
    gEditorState.active_component_id = component_index->begin()->first;
  }

  const auto& active_component_def =
      component_index->get_comp(gEditorState.active_component_id.value());
  const auto& active_component_name = active_component_def.get_name();

  if (const Combo combo {"##ComponentCombo", active_component_name.c_str()};
      combo.is_open()) {
    for (const auto& [component_id, component_def]: *component_index) {
      const auto& component_name = component_def.get_name();
      if (Selectable::property(component_name.c_str())) {
        gEditorState.active_component_id = component_def.get_uuid();
      }
    }
  }
}

void ui_component_selector_row(const Strings& lang,
                               const ComponentIndex* component_index,
                               entt::dispatcher& dispatcher)
{
  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted(lang.misc.component.c_str());
  ImGui::SameLine();

  ui_component_combo(component_index);

  ImGui::SameLine();
  if (ui_button(TAC_ICON_ADD, lang.tooltip.create_component.c_str())) {
    open_define_component_dialog();
  }

  ImGui::SameLine();
  if (ui_button(TAC_ICON_THREE_DOTS, lang.tooltip.show_component_actions.c_str())) {
    ImGui::OpenPopup(kComponentActionPopupId);
  }

  ui_component_action_popup(lang, component_index, dispatcher);
}

void ui_component_attribute_row_name_popup(const Strings& lang,
                                           const UUID& component_id,
                                           const String& attribute_name,
                                           entt::dispatcher& dispatcher)
{
  if (auto popup = Popup::for_item("##ComponentAttributeNamePopup"); popup.is_open()) {
    if (ImGui::MenuItem(lang.action.rename_attribute.c_str())) {
      open_rename_component_attribute_dialog(gEditorState.active_component_id.value(),
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
}

void ui_component_attribute_row(const Strings& lang,
                                const UUID& component_id,
                                const String& attribute_name,
                                const Attribute& attribute,
                                entt::dispatcher& dispatcher)
{
  const Scope scope {attribute_name.c_str()};

  ImGui::TableNextRow();
  ImGui::TableNextColumn();

  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted(attribute_name.c_str());

  ui_component_attribute_row_name_popup(lang, component_id, attribute_name, dispatcher);

  ImGui::TableNextColumn();
  ImGui::SetNextItemWidth(-kMinFloat);

  if (const auto new_attribute_type = ui_attribute_type_combo(attribute.get_type())) {
    dispatcher.enqueue<SetComponentAttrTypeEvent>(component_id,
                                                  attribute_name,
                                                  *new_attribute_type);
  }

  ImGui::TableNextColumn();

  if (auto updated_attribute = ui_attribute_input("##Value", attribute)) {
    dispatcher.enqueue<UpdateComponentEvent>(component_id,
                                             attribute_name,
                                             std::move(*updated_attribute));
  }
}

void ui_component_attribute_table(const Strings& lang,
                                  const ComponentDefinition& component_def,
                                  entt::dispatcher& dispatcher)
{
  if (component_def.empty()) {
    ui_centered_label(lang.misc.empty_component.c_str());
  }
  else {
    const auto table_flags = ImGuiTableFlags_PadOuterX | ImGuiTableFlags_Resizable;
    if (const Table table {"##ComponentAttributeTable", 3, table_flags};
        table.is_open()) {
      ImGui::TableSetupColumn(lang.misc.name.c_str(), ImGuiTableColumnFlags_WidthStretch);
      ImGui::TableSetupColumn(lang.misc.type.c_str(), ImGuiTableColumnFlags_WidthStretch);
      ImGui::TableSetupColumn(lang.misc.default_value.c_str(),
                              ImGuiTableColumnFlags_WidthStretch);

      ImGui::TableHeadersRow();

      for (const auto& [attribute_name, attribute]: component_def) {
        ui_component_attribute_row(lang,
                                   component_def.get_uuid(),
                                   attribute_name,
                                   attribute,
                                   dispatcher);
      }
    }
  }

  if (ui_centered_button(lang.action.create_attribute.c_str())) {
    open_create_component_attribute_dialog(gEditorState.active_component_id.value());
  }
}

}  // namespace

void open_component_editor_dialog(const DocumentModel& model)
{
  const auto* component_index = model.require_active_document().find_component_index();
  TACTILE_ASSERT(component_index != nullptr);

  gEditorState.active_component_id =
      !component_index->empty() ? Maybe<UUID> {component_index->begin()->first} : nothing;
  gEditorState.open_dialog = true;
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

  if (gEditorState.open_dialog) {
    options.flags |= UI_DIALOG_FLAG_OPEN;
    gEditorState.open_dialog = false;
  }

  if (const ScopedDialog dialog {options}; dialog.was_opened()) {
    const auto& document = model.require_active_document();
    const auto* component_index = document.find_component_index();
    TACTILE_ASSERT(component_index != nullptr);

    // Ensure that the active component ID hasn't been invalidated
    if (gEditorState.active_component_id &&
        !component_index->has_comp(*gEditorState.active_component_id)) {
      gEditorState.active_component_id.reset();
    }

    if (component_index->empty()) {
      ImGui::TextUnformatted(lang.misc.map_has_no_components.c_str());

      if (ui_centered_button(TAC_ICON_ADD, lang.tooltip.create_component.c_str())) {
        open_define_component_dialog();
      }
    }
    else {
      ui_component_selector_row(lang, component_index, dispatcher);

      ImGui::Separator();

      if (gEditorState.active_component_id) {
        const auto& active_component_def =
            component_index->get_comp(*gEditorState.active_component_id);
        ui_component_attribute_table(lang, active_component_def, dispatcher);
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
