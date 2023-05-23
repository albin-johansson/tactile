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

#include <imgui.h>

#include "common/debug/assert.hpp"
#include "common/type/maybe.hpp"
#include "components/context.hpp"
#include "components/document.hpp"
#include "components/map.hpp"
#include "model/context.hpp"
#include "model/event/layer_events.hpp"
#include "model/event/setting_events.hpp"
#include "model/model.hpp"
#include "model/systems/document_system.hpp"
#include "model/systems/group_layer_system.hpp"
#include "systems/language_system.hpp"
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
  Maybe<Entity> rename_target_layer_entity;
  bool has_focus {};
};

inline LayerDockState gDockState;

void _push_side_buttons(const Model& model,
                        const Strings& strings,
                        Dispatcher& dispatcher)
{
  const auto& document_entity = sys::get_active_document(model);
  const auto& map_document = model.get<MapDocument>(document_entity);
  const auto& map = model.get<Map>(map_document.map);
  const auto& root_layer = model.get<GroupLayer>(map.root_layer);

  const auto has_active_layer = map.active_layer != kNullEntity;
  const Group group;

  if (push_icon_button(TAC_ICON_ADD, strings.tooltip.add_new_layer.c_str())) {
    gDockState.add_layer_context_menu.show();
  }

  gDockState.add_layer_context_menu.update(model, dispatcher);

  if (push_icon_button(TAC_ICON_REMOVE,
                       strings.tooltip.remove_layer.c_str(),
                       has_active_layer)) {
    dispatcher.enqueue<RemoveLayerEvent>(map.active_layer);
  }

  if (push_icon_button(TAC_ICON_DUPLICATE,
                       strings.tooltip.duplicate_layer.c_str(),
                       has_active_layer)) {
    dispatcher.enqueue<DuplicateLayerEvent>(map.active_layer);
  }

  if (push_icon_button(TAC_ICON_MOVE_UP,
                       strings.tooltip.move_layer_up.c_str(),
                       has_active_layer &&
                           sys::can_move_layer_up(model, root_layer, map.active_layer))) {
    dispatcher.enqueue<MoveLayerUpEvent>(map.active_layer);
  }

  if (push_icon_button(
          TAC_ICON_MOVE_DOWN,
          strings.tooltip.move_layer_down.c_str(),
          has_active_layer &&
              sys::can_move_layer_down(model, root_layer, map.active_layer))) {
    dispatcher.enqueue<MoveLayerDownEvent>(map.active_layer);
  }
}

void _push_rename_dialog(const Model& model, Dispatcher& dispatcher)
{
  if (gDockState.rename_target_layer_entity.has_value()) {
    const auto target_layer_entity = *gDockState.rename_target_layer_entity;
    const auto& layer_context = model.get<Context>(target_layer_entity);

    open_rename_layer_dialog(target_layer_entity, layer_context.name);
    gDockState.rename_target_layer_entity.reset();
  }

  update_rename_layer_dialog(model, dispatcher);
}

void _push_contents(const Model& model, const Strings& strings, Dispatcher& dispatcher)
{
  _push_side_buttons(model, strings, dispatcher);

  ImGui::SameLine();
  const Group group;

  const auto document_entity = sys::get_active_document(model);
  const auto& map_document = model.get<MapDocument>(document_entity);

  const auto& map = model.get<Map>(map_document.map);
  const auto& root_layer = model.get<GroupLayer>(map.root_layer);

  if (root_layer.children.empty()) {
    prepare_vertical_alignment_center(1);
    push_centered_label(strings.misc.map_has_no_layers.c_str());
  }
  else {
    const ImVec2 size {-kMinFloat, -kMinFloat};
    if (const ListBox list {"##LayerTreeNode", size}; list.is_open()) {
      for (const auto layer_entity: root_layer.children) {
        layer_selectable(model, map_document.map, layer_entity, dispatcher);
      }
    }
  }

  _push_rename_dialog(model, dispatcher);
}

}  // namespace

void show_layer_dock(const Model& model, Entity, Dispatcher& dispatcher)
{
  TACTILE_ASSERT(sys::has_active_document(model));

  const auto& settings = model.get<Settings>();

  if (!settings.test_flag(SETTINGS_SHOW_LAYER_DOCK_BIT)) {
    return;
  }

  const auto& strings = sys::get_current_language_strings(model);

  bool show_layer_dock = true;
  const Window dock {strings.window.layer_dock.c_str(),
                     ImGuiWindowFlags_NoCollapse,
                     &show_layer_dock};

  if (show_layer_dock != settings.test_flag(SETTINGS_SHOW_LAYER_DOCK_BIT)) {
    dispatcher.enqueue<SetFlagSettingEvent>(SETTINGS_SHOW_LAYER_DOCK_BIT,
                                            show_layer_dock);
  }

  gDockState.has_focus = dock.has_focus(ImGuiFocusedFlags_RootAndChildWindows);

  if (dock.is_open()) {
    _push_contents(model, strings, dispatcher);
  }
}

auto is_layer_dock_enabled(const Model& model) -> bool
{
  return sys::is_map_document_active(model);
}

void show_rename_layer_dialog(const Entity layer_entity)
{
  gDockState.rename_target_layer_entity = layer_entity;
}

auto is_layer_dock_focused() -> bool
{
  return gDockState.has_focus;
}

}  // namespace tactile::ui
