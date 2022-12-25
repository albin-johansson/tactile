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

#include "add_layer_context_menu.hpp"
#include "core/layer/group_layer.hpp"
#include "core/type/maybe.hpp"
#include "debug/assert.hpp"
#include "io/proto/preferences.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "layer_selectable.hpp"
#include "model/document/map_document.hpp"
#include "model/event/layer_events.hpp"
#include "model/model.hpp"
#include "ui/constants.hpp"
#include "ui/dock/layer/dialogs/rename_layer_dialog.hpp"
#include "ui/style/alignment.hpp"
#include "ui/style/icons.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"

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

  const Group group;

  if (ui_icon_button(TAC_ICON_ADD, lang.tooltip.add_new_layer.c_str())) {
    add_layer_context_menu.show();
  }

  add_layer_context_menu.update(model, dispatcher);

  if (ui_icon_button(TAC_ICON_REMOVE,
                     lang.tooltip.remove_layer.c_str(),
                     has_active_layer)) {
    dispatcher.enqueue<RemoveLayerEvent>(active_layer_id.value());
  }

  if (ui_icon_button(TAC_ICON_DUPLICATE,
                     lang.tooltip.duplicate_layer.c_str(),
                     has_active_layer)) {
    dispatcher.enqueue<DuplicateLayerEvent>(active_layer_id.value());
  }

  const auto& root = map.invisible_root();

  if (ui_icon_button(TAC_ICON_MOVE_UP,
                     lang.tooltip.move_layer_up.c_str(),
                     has_active_layer && root.can_move_layer_up(*active_layer_id))) {
    dispatcher.enqueue<MoveLayerUpEvent>(active_layer_id.value());
  }

  if (ui_icon_button(TAC_ICON_MOVE_DOWN,
                     lang.tooltip.move_layer_down.c_str(),
                     has_active_layer && root.can_move_layer_down(*active_layer_id))) {
    dispatcher.enqueue<MoveLayerDownEvent>(active_layer_id.value());
  }
}

void update_rename_dialog(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  if (rename_target_id.has_value()) {
    const auto target_layer_id = *rename_target_id;

    const auto& map = model.require_active_map().get_map();
    const auto& layer = map.invisible_root().get_layer(target_layer_id);

    open_rename_layer_dialog(target_layer_id, layer.get_ctx().name());
    rename_target_id.reset();
  }

  update_rename_layer_dialog(dispatcher);
}

void update_contents(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  update_side_buttons(model, dispatcher);

  ImGui::SameLine();
  const Group group;

  const auto& document = model.require_active_map();
  const auto& root = document.get_map().invisible_root();

  if (root.layer_count() == 0) {
    const auto& lang = get_current_language();

    prepare_vertical_alignment_center(1);
    ui_centered_label(lang.misc.map_has_no_layers.c_str());
  }
  else {
    const ImVec2 size {-min_float, -min_float};
    if (const ListBox list {"##LayerTreeNode", size}; list.is_open()) {
      root.each([&](const Layer& layer) {
        if (!layer.get_parent().has_value()) {
          layer_selectable(document, layer, dispatcher);
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

  const Window dock {lang.window.layer_dock.c_str(),
                     ImGuiWindowFlags_NoCollapse,
                     &prefs.show_layer_dock};
  is_focused = dock.has_focus(ImGuiFocusedFlags_RootAndChildWindows);

  if (dock.is_open()) {
    update_contents(model, dispatcher);
  }
}

void show_rename_layer_dialog(const UUID& layer_id)
{
  rename_target_id = layer_id;
}

auto is_layer_dock_focused() -> bool
{
  return is_focused;
}

}  // namespace tactile::ui
