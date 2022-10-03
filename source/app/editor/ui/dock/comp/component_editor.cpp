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

#include <entt/entity/registry.hpp>
#include <entt/signal/dispatcher.hpp>
#include <imgui.h>

#include "core/attribute.hpp"
#include "core/comp/component_index.hpp"
#include "core/event/component_events.hpp"
#include "core/model.hpp"
#include "core/type/maybe.hpp"
#include "core/type/string.hpp"
#include "core/uuid.hpp"
#include "editor/constants.hpp"
#include "editor/ui/common/buttons.hpp"
#include "editor/ui/common/input_widgets.hpp"
#include "editor/ui/common/labels.hpp"
#include "editor/ui/dock/property/dialogs/property_type_combo.hpp"
#include "editor/ui/icons.hpp"
#include "editor/ui/scoped.hpp"
#include "editor/ui/shared/dialog_state.hpp"
#include "editor/ui/shared/dialogs.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "misc/assert.hpp"

namespace tactile::ui {

struct ComponentEditor::Data final {
  Maybe<UUID> active_component;
};

ComponentEditor::ComponentEditor()
    : Dialog {get_current_language().window.component_editor}
    , mData {std::make_unique<Data>()}
{
  set_accept_button_label(nothing);
}

ComponentEditor::~ComponentEditor() noexcept = default;

void ComponentEditor::show(const DocumentModel& model)
{
  const auto& document = model.require_active_document();
  const auto* index = document.view_component_index();
  TACTILE_ASSERT(index != nullptr);

  mData->active_component =
      !index->empty() ? Maybe<UUID> {index->begin()->first} : nothing;

  const auto& lang = get_current_language();
  set_title(lang.window.component_editor);
  set_close_button_label(lang.misc.close);

  make_visible();
}

void ComponentEditor::on_update(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  const auto& document = model.require_active_document();
  const auto* index = document.view_component_index();
  TACTILE_ASSERT(index != nullptr);

  auto& data = *mData;
  const auto& lang = get_current_language();

  /* Ensure that the active component ID hasn't been invalidated */
  if (data.active_component && !index->contains(*data.active_component)) {
    data.active_component.reset();
  }

  if (index->empty()) {
    ImGui::TextUnformatted(lang.misc.map_has_no_components.c_str());

    if (centered_button(TAC_ICON_ADD, lang.tooltip.create_component.c_str())) {
      get_dialogs().define_component.show();
    }
  }
  else {
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(lang.misc.component.c_str());
    ImGui::SameLine();

    if (!data.active_component) {
      const auto iter = index->begin();
      data.active_component = iter->first;
    }

    const auto& definition = index->at(data.active_component.value());
    const auto& name = definition.get_name();
    if (Combo combo {"##ComponentEditorCombo", name.c_str()}; combo.is_open()) {
      for (const auto& [componentId, component] : *index) {
        if (ImGui::Selectable(component.get_name().c_str())) {
          data.active_component = component.get_uuid();
        }
      }
    }

    ImGui::SameLine();

    if (button(TAC_ICON_ADD, lang.tooltip.create_component.c_str())) {
      get_dialogs().define_component.show();
    }

    ImGui::SameLine();

    if (button(TAC_ICON_THREE_DOTS, lang.tooltip.show_component_actions.c_str())) {
      ImGui::OpenPopup("##ComponentEditorPopup");
    }

    show_component_combo_popup(document, dispatcher);

    ImGui::Separator();
  }

  if (data.active_component) {
    const auto& definition = index->at(*data.active_component);
    show_component_attributes(definition, dispatcher);
  }

  auto& dialogs = get_dialogs();
  dialogs.define_component.update(model, dispatcher);
  dialogs.add_component_attr.update(model, dispatcher);
  dialogs.rename_component.update(model, dispatcher);
  dialogs.rename_component_attr.update(model, dispatcher);

  ImGui::Spacing();
  ImGui::Separator();
}

void ComponentEditor::show_component_combo_popup(const Document& document,
                                                 entt::dispatcher& dispatcher)
{
  auto& data = *mData;
  const auto& lang = get_current_language();

  if (Popup popup {"##ComponentEditorPopup"}; popup.is_open()) {
    if (ImGui::MenuItem(lang.action.rename_component.c_str())) {
      const auto id = data.active_component.value();
      const auto* index = document.view_component_index();
      const auto& name = index->at(id).get_name();
      get_dialogs().rename_component.show(name, id);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(lang.action.remove_component.c_str())) {
      dispatcher.enqueue<UndefComponentEvent>(data.active_component.value());
      data.active_component.reset();
    }
  }
}

void ComponentEditor::show_component_attributes(const ComponentDefinition& definition,
                                                entt::dispatcher& dispatcher)
{
  auto& data = *mData;
  const auto& lang = get_current_language();

  if (definition.empty()) {
    centered_label(lang.misc.empty_component.c_str());
  }
  else {
    constexpr auto table_flags = ImGuiTableFlags_PadOuterX | ImGuiTableFlags_Resizable;
    if (Table table {"##ComponentAttributeTable", 3, table_flags}; table.is_open()) {
      ImGui::TableSetupColumn(lang.misc.name.c_str(), ImGuiTableColumnFlags_WidthStretch);
      ImGui::TableSetupColumn(lang.misc.type.c_str(), ImGuiTableColumnFlags_WidthStretch);
      ImGui::TableSetupColumn(lang.misc.default_value.c_str(),
                              ImGuiTableColumnFlags_WidthStretch);
      ImGui::TableHeadersRow();

      for (const auto& [name, attr] : definition) {
        show_component_attribute(definition.get_uuid(), name, attr, dispatcher);
      }
    }
  }

  if (centered_button(lang.action.create_attribute.c_str())) {
    get_dialogs().add_component_attr.show(*data.active_component);
  }
}

void ComponentEditor::show_component_attribute(const UUID& component_id,
                                               const String& name,
                                               const Attribute& value,
                                               entt::dispatcher& dispatcher)
{
  const auto& lang = get_current_language();
  auto& data = *mData;

  const Scope scope {name.c_str()};

  ImGui::TableNextRow();
  ImGui::TableNextColumn();

  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted(name.c_str());

  if (auto popup = Popup::for_item("##ComponentAttributeNameContext"); popup.is_open()) {
    if (ImGui::MenuItem(lang.action.rename_attribute.c_str())) {
      get_dialogs().rename_component_attr.show(name, data.active_component.value());
    }

    ImGui::Separator();

    if (ImGui::MenuItem(lang.action.duplicate_attribute.c_str())) {
      dispatcher.enqueue<DuplicateComponentAttrEvent>(component_id, name);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(lang.action.remove_attribute.c_str())) {
      dispatcher.enqueue<RemoveComponentAttrEvent>(component_id, name);
    }
  }

  ImGui::TableNextColumn();
  ImGui::SetNextItemWidth(-min_float);

  const auto type = value.type();
  AttributeType new_type = type;
  show_property_type_combo(type, new_type);
  if (new_type != type) {
    dispatcher.enqueue<SetComponentAttrTypeEvent>(component_id, name, new_type);
  }

  ImGui::TableNextColumn();

  if (auto updated = input_attribute("##DefaultValue", value)) {
    dispatcher.enqueue<UpdateComponentEvent>(component_id, name, std::move(*updated));
  }
}

}  // namespace tactile::ui
