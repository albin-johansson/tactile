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

#include <utility>  // move

#include <imgui.h>

#include "core/components/attributes.hpp"
#include "core/systems/component_system.hpp"
#include "core/systems/context_system.hpp"
#include "editor/events/component_events.hpp"
#include "editor/gui/alignment.hpp"
#include "editor/gui/common/button.hpp"
#include "editor/gui/common/centered_text.hpp"
#include "editor/gui/common/input_widgets.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/scoped.hpp"
#include "editor/model.hpp"
#include "io/persistence/preferences.hpp"
#include "tactile.hpp"

namespace tactile {
namespace {

constexpr auto _window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;

[[nodiscard]] auto _show_trailing_component_button() -> bool
{
  ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32_BLACK_TRANS);
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32_BLACK_TRANS);
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32_BLACK_TRANS);

  right_align_next_item(TAC_ICON_THREE_DOTS);
  const auto pressed = ImGui::SmallButton(TAC_ICON_THREE_DOTS);

  ImGui::PopStyleColor(3);

  return pressed;
}

void _show_component_popup_content(entt::dispatcher& dispatcher,
                                   const context_id contextId,
                                   const component_id componentId)
{
  if (ImGui::MenuItem(TAC_ICON_RESET " Reset Values")) {
    dispatcher.enqueue<ResetComponentValuesEvent>(contextId, componentId);
  }

  ImGui::Separator();

  if (ImGui::MenuItem(TAC_ICON_COPY " Copy Component")) {
    // TODO
  }

  ImGui::Separator();

  if (ImGui::MenuItem(TAC_ICON_REMOVE " Remove Component")) {
    dispatcher.enqueue<RemoveComponentEvent>(contextId, componentId);
  }
}

void _show_component(entt::dispatcher& dispatcher,
                     const context_id contextId,
                     const char* name,
                     const comp::Component& component)
{
  const scoped::Id componentScope{name};

  constexpr auto headerFlags =
      ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;

  if (scoped::TreeNode node{name, headerFlags}; node.is_open()) {
    ImGui::SameLine();
    if (_show_trailing_component_button()) {
      ImGui::OpenPopup("##ComponentPopup");
    }

    if (const auto popup = scoped::Popup::for_item("##ComponentPopup"); popup.is_open()) {
      _show_component_popup_content(dispatcher, contextId, component.type);
    }

    constexpr auto tableFlags =
        ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable | ImGuiTableFlags_PadOuterX;

    if (scoped::Table table{"##ComponentAttributeTable", 2, tableFlags};
        table.is_open()) {
      for (const auto& [attributeName, attribute] : component.values) {
        const scoped::Id attributeScope{attributeName.c_str()};

        ImGui::TableNextRow();
        ImGui::TableNextColumn();

        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted(attributeName.c_str());

        ImGui::TableNextColumn();

        if (auto value = input_attribute("##ComponentAttributeTableValue", attribute)) {
          dispatcher.enqueue<UpdateComponentEvent>(contextId,
                                                   component.type,
                                                   attributeName,
                                                   std::move(*value));
        }
      }
    }
  }
}

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

  if (scoped::Child pane{"##ComponentsChild"}; pane.is_open()) {
    if (context.components.empty()) {
      prepare_vertical_alignment_center(2);
      centered_text("There are no components associated with the current context.");
    }
    else {
      for (const auto componentEntity : context.components) {
        const auto& component = registry.get<comp::Component>(componentEntity);
        const auto& name = sys::get_component_def_name(registry, component.type);

        ImGui::Separator();
        _show_component(dispatcher, context.id, name.c_str(), component);
      }

      ImGui::Separator();
    }

    if (centered_button(TAC_ICON_ADD, "Add component")) {
      ImGui::OpenPopup("##AddComponentButtonPopup");
    }

    if (scoped::Popup popup{"##AddComponentButtonPopup"}; popup.is_open()) {
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
