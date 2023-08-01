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

#include "component_dock.hpp"

#include <imgui.h>

#include "core/formatters/fmt_string.hpp"
#include "model/components/component_components.hpp"
#include "model/contexts/attached_component_system.hpp"
#include "model/contexts/context_components.hpp"
#include "model/documents/document_components.hpp"
#include "model/documents/document_system.hpp"
#include "model/events/component_events.hpp"
#include "model/events/setting_events.hpp"
#include "ui/dock/comp/component_tree_node.hpp"
#include "ui/style/alignment.hpp"
#include "ui/style/icons.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile {
namespace {

inline constexpr auto kAddComponentPopupId = "##AddComponentButtonPopup";

void _push_add_component_button_popup_content(ModelView model,
                                              const Entity document_entity)
{
  const auto& strings = model.get_language_strings();
  const auto& registry = model.get_registry();

  const auto& document = registry.get<Document>(document_entity);
  const auto& component_set = registry.get<ComponentSet>(document.component_set);

  if (component_set.definitions.empty()) {
    const ui::Disable disable;
    ImGui::TextUnformatted(strings.misc.no_available_components.c_str());
  }
  else {
    const auto& active_context = registry.get<Context>(document.active_context);

    for (const auto component_entity: component_set.definitions) {
      const auto& component = registry.get<Component>(component_entity);

      const ui::Disable disable_if {
          sys::has_component(registry, active_context, component.name)};

      if (ImGui::MenuItem(component.name.c_str())) {
        model.enqueue<AttachComponentEvent>(document.active_context, component_entity);
      }
    }
  }

  ImGui::Separator();
  if (ImGui::MenuItem(strings.action.component_editor.c_str())) {
    model.enqueue<ShowComponentEditorEvent>();
  }
}

void _push_dock_contents(ModelView model, const Entity document_entity)
{
  const auto& registry = model.get_registry();
  const auto& strings = model.get_language_strings();

  const auto& document = registry.get<Document>(document_entity);
  const auto active_context_entity = document.get_active_context();
  const auto& active_context = registry.get<Context>(active_context_entity);

  const FmtString indicator {"{}: {}", strings.misc.context, active_context.name};
  ImGui::TextUnformatted(indicator.data());

  if (const ui::Child pane {"##ComponentsChild"}; pane.is_open()) {
    if (active_context.comps.empty()) {
      ui::prepare_vertical_alignment_center(2);
      ui::push_centered_label(strings.misc.context_has_no_components.c_str());
    }
    else {
      for (const auto attached_component_entity: active_context.comps) {
        ImGui::Separator();
        push_component_tree_node(model, active_context_entity, attached_component_entity);
      }

      ImGui::Separator();
    }

    if (ui::push_centered_button(TAC_ICON_ADD, strings.tooltip.add_component.c_str())) {
      ImGui::OpenPopup(kAddComponentPopupId);
    }

    if (const ui::Popup popup {kAddComponentPopupId}; popup.is_open()) {
      _push_add_component_button_popup_content(model, document_entity);
    }
  }
}

}  // namespace

void push_component_dock_widget(ModelView model)
{
  const auto& strings = model.get_language_strings();
  const auto& settings = model.get_settings();

  if (!settings.test_flag(SETTINGS_SHOW_COMPONENT_DOCK_BIT)) {
    return;
  }

  bool show_component_dock = true;
  const ui::Window dock {strings.window.component_dock.c_str(),
                         ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar,
                         &show_component_dock};

  if (show_component_dock != settings.test_flag(SETTINGS_SHOW_COMPONENT_DOCK_BIT)) {
    model.enqueue<SetFlagSettingEvent>(SETTINGS_SHOW_COMPONENT_DOCK_BIT,
                                       show_component_dock);
  }

  if (dock.is_open()) {
    const auto& registry = model.get_registry();
    const auto document_entity = sys::get_active_document(registry);
    _push_dock_contents(model, document_entity);
  }
}

}  // namespace tactile
