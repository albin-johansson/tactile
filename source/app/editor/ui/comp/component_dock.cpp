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

#include "component_dock.hpp"

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>

#include "core/comp/component_bundle.hpp"
#include "core/comp/component_index.hpp"
#include "core/ctx/context.hpp"
#include "core/ctx/context_manager.hpp"
#include "core/event/component_events.hpp"
#include "core/model.hpp"
#include "core/util/fmt.hpp"
#include "editor/lang/language.hpp"
#include "editor/lang/strings.hpp"
#include "editor/ui/alignment.hpp"
#include "editor/ui/common/buttons.hpp"
#include "editor/ui/common/labels.hpp"
#include "editor/ui/comp/component_view.hpp"
#include "editor/ui/icons.hpp"
#include "editor/ui/scoped.hpp"
#include "io/persist/preferences.hpp"
#include "misc/assert.hpp"

namespace tactile::ui {
namespace {

constexpr auto add_component_popup_id = "##AddComponentButtonPopup";

void show_add_component_button_popup_content(const ADocument&  document,
                                             const IContext&   context,
                                             entt::dispatcher& dispatcher)
{
  const auto& lang = get_current_language();
  const auto* index = document.view_component_index();
  TACTILE_ASSERT(index != nullptr);

  if (index->empty()) {
    Disable disable;
    ImGui::TextUnformatted(lang.misc.no_available_components.c_str());
  }
  else {
    const auto& comps = context.get_comps();
    for (const auto& [definition_id, definition] : *index) {
      Disable disable_if {comps.contains(definition_id)};

      if (ImGui::MenuItem(definition.get_name().c_str())) {
        dispatcher.enqueue<AttachComponentEvent>(context.get_uuid(), definition_id);
      }
    }
  }

  ImGui::Separator();
  if (ImGui::MenuItem(lang.action.component_editor.c_str())) {
    dispatcher.enqueue<OpenComponentEditorEvent>();
  }
}

void show_contents(const ADocument& document, entt::dispatcher& dispatcher)
{
  const auto& lang = get_current_language();
  const auto& context = document.get_contexts().active_context();

  const FmtString indicator {"{}: {}", lang.misc.context, context.get_name()};
  ImGui::TextUnformatted(indicator.data());

  if (Child pane {"##ComponentsChild"}; pane.is_open()) {
    const auto& comps = context.get_comps();
    if (comps.empty()) {
      prepare_vertical_alignment_center(2);
      centered_label(lang.misc.context_has_no_components.c_str());
    }
    else {
      const auto* index = document.view_component_index();
      TACTILE_ASSERT(index != nullptr);

      for (const auto& [component_id, component] : comps) {
        ImGui::Separator();

        const auto& component_name = index->at(component_id).get_name();
        component_view(context.get_uuid(), component, component_name, dispatcher);
      }

      ImGui::Separator();
    }

    if (centered_button(TAC_ICON_ADD, lang.tooltip.add_component.c_str())) {
      ImGui::OpenPopup(add_component_popup_id);
    }

    if (Popup popup {add_component_popup_id}; popup.is_open()) {
      show_add_component_button_popup_content(document, context, dispatcher);
    }
  }
}

}  // namespace

void update_component_dock(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  auto& prefs = io::get_preferences();

  if (!prefs.show_component_dock) {
    return;
  }

  const auto& lang = get_current_language();

  Window dock {lang.window.component_dock.c_str(),
               ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar,
               &prefs.show_component_dock};

  if (dock.is_open()) {
    const auto& document = model.require_active_document();
    show_contents(document, dispatcher);
  }
}

}  // namespace tactile::ui
