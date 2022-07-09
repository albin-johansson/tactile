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

#include "core/common/identifiers.hpp"
#include "core/components/component_bundle.hpp"
#include "core/components/component_index.hpp"
#include "core/contexts/context.hpp"
#include "core/events/component_events.hpp"
#include "core/model.hpp"
#include "editor/ui/alignment.hpp"
#include "editor/ui/common/buttons.hpp"
#include "editor/ui/common/labels.hpp"
#include "editor/ui/components/component_view.hpp"
#include "editor/ui/icons.hpp"
#include "editor/ui/scoped.hpp"
#include "io/persistence/preferences.hpp"
#include "misc/assert.hpp"

namespace tactile::ui {
namespace {

constexpr auto _add_component_popup_id = "##AddComponentButtonPopup";

void _show_add_component_button_popup_content(const ADocument&  document,
                                              const IContext&   context,
                                              entt::dispatcher& dispatcher)
{
  const auto index = document.get_component_index();
  TACTILE_ASSERT(index != nullptr);

  if (index->empty()) {
    Disable disable;
    ImGui::TextUnformatted("No available components");
  }
  else {
    const auto& comps = context.get_comps();
    for (const auto& [definitionId, definition] : *index) {
      Disable disableIf {comps.contains(definitionId)};

      if (ImGui::MenuItem(definition.get_name().c_str())) {
        dispatcher.enqueue<AttachComponentEvent>(context.get_uuid(), definitionId);
      }
    }
  }

  ImGui::Separator();
  if (ImGui::MenuItem(TAC_ICON_COMPONENT " Component Editor...")) {
    dispatcher.enqueue<OpenComponentEditorEvent>();
  }
}

void _show_contents(const ADocument& document, entt::dispatcher& dispatcher)
{
  const auto  contextId = document.active_context_id();
  const auto& context = document.view_context(contextId);
  ImGui::Text("Context: %s", context.get_name().c_str());

  if (Child pane {"##ComponentsChild"}; pane.is_open()) {
    const auto& comps = context.get_comps();
    if (comps.empty()) {
      prepare_vertical_alignment_center(2);
      centered_label("This context has no components.");
    }
    else {
      const auto index = document.get_component_index();
      TACTILE_ASSERT(index != nullptr);

      for (const auto& [componentId, component] : comps) {
        ImGui::Separator();

        const auto& componentName = index->at(componentId).get_name();
        component_view(contextId, component, componentName, dispatcher);
      }

      ImGui::Separator();
    }

    if (centered_button(TAC_ICON_ADD, "Add component")) {
      ImGui::OpenPopup(_add_component_popup_id);
    }

    if (Popup popup {_add_component_popup_id}; popup.is_open()) {
      _show_add_component_button_popup_content(document, context, dispatcher);
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

  constexpr auto flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;

  if (Window dock {"Components", flags, &prefs.show_component_dock}; dock.is_open()) {
    const auto& document = model.require_active_document();
    _show_contents(document, dispatcher);
  }
}

}  // namespace tactile::ui
