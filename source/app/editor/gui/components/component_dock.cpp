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

#include <imgui.h>

#include "core/components/attributes.hpp"
#include "core/systems/component_system.hpp"
#include "core/systems/context_system.hpp"
#include "editor/events/component_events.hpp"
#include "editor/gui/alignment.hpp"
#include "editor/gui/common/button.hpp"
#include "editor/gui/common/centered_text.hpp"
#include "editor/gui/components/views/component_view.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/scoped.hpp"
#include "editor/model.hpp"
#include "io/persistence/preferences.hpp"
#include "tactile.hpp"

namespace tactile {
namespace {

constexpr auto _add_component_popup_id = "##AddComponentButtonPopup";

constexpr auto _window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;

void _show_add_component_button_popup_content(const entt::registry& registry,
                                              entt::dispatcher& dispatcher,
                                              const context_id contextId)
{
  const auto view = registry.view<comp::ComponentDef>();
  if (view.empty()) {
    scoped::Disable disable;
    ImGui::TextUnformatted("No available components");
  }
  else {
    for (auto [defEntity, def] : view.each()) {
      scoped::Disable disable{sys::has_component(registry, contextId, def.id)};

      if (ImGui::MenuItem(def.name.c_str())) {
        dispatcher.enqueue<AddComponentEvent>(contextId, def.id);
      }
    }
  }

  ImGui::Separator();
  if (ImGui::MenuItem(TAC_ICON_COMPONENT " Component Editor...")) {
    dispatcher.enqueue<OpenComponentEditorEvent>();
  }
}

}  // namespace

ComponentDock::ComponentDock() : ADockWidget{"Components", _window_flags}
{
  set_close_button_enabled(true);
}

void ComponentDock::on_update(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  const auto& registry = model.get_active_registry();

  const auto& context = sys::current_context(registry);
  ImGui::Text("Context: %s", context.name.c_str());

  if (scoped::Child pane{"##Components"}; pane.is_open()) {
    if (context.components.empty()) {
      prepare_vertical_alignment_center(2);
      centered_text("There are no components associated with the current context.");
    }
    else {
      for (const auto componentEntity : context.components) {
        ImGui::Separator();
        component_view(registry, dispatcher, context.id, componentEntity);
      }

      ImGui::Separator();
    }

    if (centered_button(TAC_ICON_ADD, "Add component")) {
      ImGui::OpenPopup(_add_component_popup_id);
    }

    if (scoped::Popup popup{_add_component_popup_id}; popup.is_open()) {
      _show_add_component_button_popup_content(registry, dispatcher, context.id);
    }
  }
}

void ComponentDock::set_visible(const bool visible)
{
  auto& prefs = get_preferences();
  prefs.set_component_dock_visible(visible);
}

auto ComponentDock::is_visible() const -> bool
{
  return get_preferences().is_component_dock_visible();
}

}  // namespace tactile
