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

#include "common/debug/assert.hpp"
#include "common/util/fmt.hpp"
#include "component_view.hpp"
#include "core/component.hpp"
#include "core/context.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/document.hpp"
#include "model/event/component_events.hpp"
#include "model/model.hpp"
#include "model/settings.hpp"
#include "model/systems/context/components.hpp"
#include "model/systems/document_system.hpp"
#include "ui/style/alignment.hpp"
#include "ui/style/icons.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile::ui {
namespace {

inline constexpr auto kAddComponentPopupId = "##AddComponentButtonPopup";

void _show_add_component_button_popup_content(const Model& model,
                                              const Entity document_entity,
                                              Dispatcher& dispatcher)
{
  const auto& lang = get_current_language();

  const auto& document = model.get<Document>(document_entity);
  const auto& component_set = model.get<ComponentSet>(document.component_set);

  if (component_set.definitions.empty()) {
    const Disable disable;
    ImGui::TextUnformatted(lang.misc.no_available_components.c_str());
  }
  else {
    const auto& active_context = model.get<Context>(document.active_context);

    for (const auto definition_entity: component_set.definitions) {
      const auto& definition = model.get<ComponentDefinition>(definition_entity);

      const Disable disable_if {
          sys::has_component(model, active_context, definition.name)};

      if (ImGui::MenuItem(definition.name.c_str())) {
        dispatcher.enqueue<AttachComponentEvent>(document.active_context,
                                                 definition_entity);
      }
    }
  }

  ImGui::Separator();
  if (ImGui::MenuItem(lang.action.component_editor.c_str())) {
    dispatcher.enqueue<OpenComponentEditorEvent>();
  }
}

void _show_dock_contents(const Model& model,
                         const Entity document_entity,
                         Dispatcher& dispatcher)
{
  const auto& lang = get_current_language();
  const auto& document = model.get<Document>(document_entity);

  const auto active_context_entity = document.get_active_context();
  const auto& active_context = model.get<Context>(active_context_entity);

  const FmtString indicator {"{}: {}", lang.misc.context, active_context.name};
  ImGui::TextUnformatted(indicator.data());

  if (const Child pane {"##ComponentsChild"}; pane.is_open()) {
    if (active_context.comps.empty()) {
      prepare_vertical_alignment_center(2);
      ui_centered_label(lang.misc.context_has_no_components.c_str());
    }
    else {
      for (const auto component_entity: active_context.comps) {
        ImGui::Separator();
        component_view(model, active_context_entity, component_entity, dispatcher);
      }

      ImGui::Separator();
    }

    if (ui_centered_button(TAC_ICON_ADD, lang.tooltip.add_component.c_str())) {
      ImGui::OpenPopup(kAddComponentPopupId);
    }

    if (const Popup popup {kAddComponentPopupId}; popup.is_open()) {
      _show_add_component_button_popup_content(model, document_entity, dispatcher);
    }
  }
}

}  // namespace

void show_component_dock(const Model& model, Entity, Dispatcher& dispatcher)
{
  auto& settings = get_settings();

  if (!settings.test_flag(SETTINGS_SHOW_COMPONENT_DOCK_BIT)) {
    return;
  }

  const auto& lang = get_current_language();

  bool show_component_dock = true;
  const Window dock {lang.window.component_dock.c_str(),
                     ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar,
                     &show_component_dock};
  settings.set_flag(SETTINGS_SHOW_COMPONENT_DOCK_BIT, show_component_dock);

  if (dock.is_open()) {
    const auto document_entity = sys::get_active_document(model);
    _show_dock_contents(model, document_entity, dispatcher);
  }
}

}  // namespace tactile::ui
