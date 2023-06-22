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
#include "model/documents/document_components.hpp"
#include "model/documents/document_system.hpp"
#include "model/event/layer_events.hpp"
#include "model/event/setting_events.hpp"
#include "model/layers/layer_tree_system.hpp"
#include "model/maps/map_components.hpp"
#include "model/model.hpp"
#include "model/systems/language_system.hpp"
#include "ui/constants.hpp"
#include "ui/dock/layer/dialogs/rename_layer_dialog.hpp"
#include "ui/dock/layer/layer_selectable.hpp"
#include "ui/style/alignment.hpp"
#include "ui/style/icons.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile::ui {
namespace {

void _push_side_buttons(const Model& model,
                        const Strings& strings,
                        LayerDockState& state,
                        Dispatcher& dispatcher)
{
  const auto& document_entity = sys::get_active_document(model);

  const auto& map_document = model.get<MapDocument>(document_entity);
  const auto& map = model.get<Map>(map_document.map);
  const auto& root_layer = model.get<GroupLayer>(map.root_layer);

  const auto has_active_layer = map.active_layer != kNullEntity;
  const Group group;

  if (push_icon_button(TAC_ICON_ADD, strings.tooltip.add_new_layer.c_str())) {
    state.add_layer_context_menu.show();
  }

  state.add_layer_context_menu.update(model, dispatcher);

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

void _push_contents(const Model& model,
                    const Strings& strings,
                    LayerDockState& state,
                    Dispatcher& dispatcher)
{
  _push_side_buttons(model, strings, state, dispatcher);

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

  push_rename_layer_dialog(model, state.rename_layer_dialog, dispatcher);
}

}  // namespace

void push_layer_dock_widget(const Model& model,
                            LayerDockState& state,
                            Dispatcher& dispatcher)
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

  state.has_focus = dock.has_focus(ImGuiFocusedFlags_RootAndChildWindows);

  if (dock.is_open()) {
    _push_contents(model, strings, state, dispatcher);
  }
}

auto is_layer_dock_enabled(const Model& model) -> bool
{
  return sys::is_map_document_active(model);
}

}  // namespace tactile::ui
