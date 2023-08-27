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

#include "layer_dock.hpp"

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>

#include "core/layer/group_layer.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/document/map_document.hpp"
#include "model/event/layer_events.hpp"
#include "model/model.hpp"
#include "model/settings.hpp"
#include "tactile/core/debug/assert.hpp"
#include "tactile/core/type/maybe.hpp"
#include "ui/constants.hpp"
#include "ui/dock/layer/add_layer_context_menu.hpp"
#include "ui/dock/layer/dialogs/rename_layer_dialog.hpp"
#include "ui/dock/layer/layer_selectable.hpp"
#include "ui/style/alignment.hpp"
#include "ui/style/icons.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile::ui {
namespace {

struct LayerDockState final {
  AddLayerContextMenu add_layer_context_menu;
  Maybe<UUID> rename_target_id;
  bool has_focus {};
};

inline LayerDockState gDockState;

void update_side_buttons(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  const auto& lang = get_current_language();

  const auto& document = model.require_active_map_document();
  const auto& map = document.get_map();

  const auto active_layer_id = map.get_active_layer_id();
  const auto has_active_layer = active_layer_id.has_value();

  const Group group;

  if (ui_icon_button(TAC_ICON_ADD, lang.tooltip.add_new_layer.c_str())) {
    gDockState.add_layer_context_menu.show();
  }

  gDockState.add_layer_context_menu.update(model, dispatcher);

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

  const auto& root = map.get_invisible_root();

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
  if (gDockState.rename_target_id.has_value()) {
    const auto target_layer_id = *gDockState.rename_target_id;

    const auto& map = model.require_active_map_document().get_map();
    const auto& layer = map.get_invisible_root().get_layer(target_layer_id);

    open_rename_layer_dialog(target_layer_id, layer.get_ctx().name());
    gDockState.rename_target_id.reset();
  }

  update_rename_layer_dialog(dispatcher);
}

void update_contents(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  update_side_buttons(model, dispatcher);

  ImGui::SameLine();
  const Group group;

  const auto& document = model.require_active_map_document();
  const auto& root = document.get_map().get_invisible_root();

  if (root.layer_count() == 0) {
    const auto& lang = get_current_language();

    prepare_vertical_alignment_center(1);
    ui_centered_label(lang.misc.map_has_no_layers.c_str());
  }
  else {
    const ImVec2 size {-kMinFloat, -kMinFloat};
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

  auto& settings = get_settings();

  if (!settings.test_flag(SETTINGS_SHOW_LAYER_DOCK_BIT)) {
    return;
  }

  const auto& lang = get_current_language();

  bool show_layer_dock = true;
  const Window dock {lang.window.layer_dock.c_str(),
                     ImGuiWindowFlags_NoCollapse,
                     &show_layer_dock};
  settings.set_flag(SETTINGS_SHOW_LAYER_DOCK_BIT, show_layer_dock);

  gDockState.has_focus = dock.has_focus(ImGuiFocusedFlags_RootAndChildWindows);

  if (dock.is_open()) {
    update_contents(model, dispatcher);
  }
}

void show_rename_layer_dialog(const UUID& layer_id)
{
  gDockState.rename_target_id = layer_id;
}

auto is_layer_dock_focused() -> bool
{
  return gDockState.has_focus;
}

}  // namespace tactile::ui
