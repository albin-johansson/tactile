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

#include "common/debug/assert.hpp"
#include "common/util/fmt.hpp"
#include "component_view.hpp"
#include "core/component/component_index.hpp"
#include "core/context/context.hpp"
#include "core/context/context_info.hpp"
#include "core/context/context_manager.hpp"
#include "io/proto/preferences.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/event/component_events.hpp"
#include "model/model.hpp"
#include "ui/style/alignment.hpp"
#include "ui/style/icons.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile::ui {
namespace {

constexpr auto add_component_popup_id = "##AddComponentButtonPopup";

void show_add_component_button_popup_content(const Document& document,
                                             const Context& context,
                                             entt::dispatcher& dispatcher)
{
  const auto& lang = get_current_language();
  const auto* component_index = document.find_component_index();
  TACTILE_ASSERT(component_index != nullptr);

  if (component_index->empty()) {
    const Disable disable;
    ImGui::TextUnformatted(lang.misc.no_available_components.c_str());
  }
  else {
    const auto& ctx = context.get_ctx();
    for (const auto& [component_id, component_def]: *component_index) {
      const Disable disable_if {ctx.has_component(component_id)};
      if (ImGui::MenuItem(component_def.name().c_str())) {
        dispatcher.enqueue<AttachComponentEvent>(context.get_uuid(), component_id);
      }
    }
  }

  ImGui::Separator();
  if (ImGui::MenuItem(lang.action.component_editor.c_str())) {
    dispatcher.enqueue<OpenComponentEditorEvent>();
  }
}

void show_contents(const Document& document, entt::dispatcher& dispatcher)
{
  const auto& lang = get_current_language();
  const auto& context = document.get_contexts().active_context();

  const FmtString indicator {"{}: {}", lang.misc.context, context.get_ctx().name()};
  ImGui::TextUnformatted(indicator.data());

  if (const Child pane {"##ComponentsChild"}; pane.is_open()) {
    const auto& ctx = context.get_ctx();
    if (ctx.component_count() == 0) {
      prepare_vertical_alignment_center(2);
      ui_centered_label(lang.misc.context_has_no_components.c_str());
    }
    else {
      const auto* component_index = document.find_component_index();
      TACTILE_ASSERT(component_index != nullptr);

      ctx.each_component([&](const UUID& component_id, const Component& component) {
        ImGui::Separator();

        const auto& component_name = component_index->at(component_id).name();
        component_view(context.get_uuid(), component, component_name, dispatcher);
      });

      ImGui::Separator();
    }

    if (ui_centered_button(TAC_ICON_ADD, lang.tooltip.add_component.c_str())) {
      ImGui::OpenPopup(add_component_popup_id);
    }

    if (const Popup popup {add_component_popup_id}; popup.is_open()) {
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

  const Window dock {lang.window.component_dock.c_str(),
                     ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar,
                     &prefs.show_component_dock};

  if (dock.is_open()) {
    const auto& document = model.require_active_document();
    show_contents(document, dispatcher);
  }
}

}  // namespace tactile::ui
