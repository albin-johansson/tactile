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

#include "layer_dock.hpp"

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>

#include "core/common/maybe.hpp"
#include "core/document/map_document.hpp"
#include "core/event/layer_events.hpp"
#include "core/model.hpp"
#include "editor/constants.hpp"
#include "editor/ui/alignment.hpp"
#include "editor/ui/common/buttons.hpp"
#include "editor/ui/common/labels.hpp"
#include "editor/ui/icons.hpp"
#include "editor/ui/layer/add_layer_context_menu.hpp"
#include "editor/ui/layer/layer_selectable.hpp"
#include "editor/ui/scoped.hpp"
#include "editor/ui/shared/dialog_state.hpp"
#include "editor/ui/shared/dialogs.hpp"
#include "io/persist/preferences.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "misc/assert.hpp"

namespace tactile::ui {
namespace {

inline AddLayerContextMenu add_layer_context_menu;
inline Maybe<UUID> rename_target_id;
constinit bool is_focused = false;

void update_side_buttons(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  const auto& lang = get_current_language();

  const auto& document = model.require_active_map();
  const auto& map = document.get_map();

  const auto active_layer_id = map.active_layer_id();
  const auto has_active_layer = active_layer_id.has_value();

  Group group;

  if (icon_button(TAC_ICON_ADD, lang.tooltip.add_new_layer.c_str())) {
    add_layer_context_menu.show();
  }

  add_layer_context_menu.update(model, dispatcher);

  if (icon_button(TAC_ICON_REMOVE, lang.tooltip.remove_layer.c_str(), has_active_layer)) {
    dispatcher.enqueue<RemoveLayerEvent>(active_layer_id.value());
  }

  if (icon_button(TAC_ICON_DUPLICATE,
                  lang.tooltip.duplicate_layer.c_str(),
                  has_active_layer)) {
    dispatcher.enqueue<DuplicateLayerEvent>(active_layer_id.value());
  }

  if (icon_button(TAC_ICON_MOVE_UP,
                  lang.tooltip.move_layer_up.c_str(),
                  has_active_layer && map.can_move_layer_up(*active_layer_id))) {
    dispatcher.enqueue<MoveLayerUpEvent>(active_layer_id.value());
  }

  if (icon_button(TAC_ICON_MOVE_DOWN,
                  lang.tooltip.move_layer_down.c_str(),
                  has_active_layer && map.can_move_layer_down(*active_layer_id))) {
    dispatcher.enqueue<MoveLayerDownEvent>(active_layer_id.value());
  }
}

void update_rename_dialog(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  auto& dialogs = get_dialogs();

  if (rename_target_id.has_value()) {
    const auto target_layer_id = *rename_target_id;

    const auto& document = model.require_active_map();
    const auto& map = document.get_map();
    const auto& layer = map.view_layer(target_layer_id);

    dialogs.rename_layer.show(target_layer_id, layer.ctx().name());
    rename_target_id.reset();
  }

  dialogs.rename_layer.update(model, dispatcher);
}

void update_contents(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  update_side_buttons(model, dispatcher);

  ImGui::SameLine();
  Group group;

  const auto& document = model.require_active_map();
  const auto& map = document.get_map();

  if (map.layer_count() == 0) {
    const auto& lang = get_current_language();

    prepare_vertical_alignment_center(1);
    centered_label(lang.misc.map_has_no_layers.c_str());
  }
  else {
    const ImVec2 size {-min_float, -min_float};
    if (ListBox list {"##LayerTreeNode", size}; list.is_open()) {
      map.visit_layers([&](const ILayer* layer) {
        if (!layer->get_parent().has_value()) {
          layer_selectable(document, *layer, dispatcher);
        }
      });
    }
  }

  update_rename_dialog(model, dispatcher);
}

}  // namespace

void update_layer_dock(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  TACTILE_ASSERT(model.has_active_document());

  auto& prefs = io::get_preferences();

  if (!prefs.show_layer_dock) {
    return;
  }

  const auto& lang = get_current_language();

  Window dock {lang.window.layer_dock.c_str(),
               ImGuiWindowFlags_NoCollapse,
               &prefs.show_layer_dock};
  is_focused = dock.has_focus(ImGuiFocusedFlags_RootAndChildWindows);

  if (dock.is_open()) {
    update_contents(model, dispatcher);
  }
}

void show_rename_layer_dialog(const UUID& layerId)
{
  rename_target_id = layerId;
}

auto is_layer_dock_focused() -> bool
{
  return is_focused;
}

}  // namespace tactile::ui
