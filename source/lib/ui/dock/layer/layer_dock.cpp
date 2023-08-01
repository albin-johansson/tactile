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

#include "core/debug/assert.hpp"
#include "model/documents/document_components.hpp"
#include "model/documents/document_system.hpp"
#include "model/events/layer_events.hpp"
#include "model/events/setting_events.hpp"
#include "model/layers/layer_tree_system.hpp"
#include "model/maps/map_components.hpp"
#include "model/registry.hpp"
#include "model/services/language_service.hpp"
#include "model/services/service_locator.hpp"
#include "ui/constants.hpp"
#include "ui/dock/layer/dialogs/rename_layer_dialog.hpp"
#include "ui/dock/layer/layer_selectable.hpp"
#include "ui/style/alignment.hpp"
#include "ui/style/icons.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile {
namespace {

void _push_side_buttons(ModelView& model, const Strings& strings, LayerDockState& state)
{
  auto& dispatcher = model.get_dispatcher();
  const auto& registry = model.get_registry();
  const auto& document_entity = sys::get_active_document(registry);

  const auto& map_document = registry.get<MapDocument>(document_entity);
  const auto& map = registry.get<Map>(map_document.map);
  const auto& root_layer = registry.get<GroupLayer>(map.root_layer);

  const auto has_active_layer = map.active_layer != kNullEntity;
  const ui::Group group;

  if (ui::push_icon_button(TAC_ICON_ADD, strings.tooltip.add_new_layer.c_str())) {
    state.add_layer_context_menu.show();
  }

  state.add_layer_context_menu.update(registry, dispatcher);

  if (ui::push_icon_button(TAC_ICON_REMOVE,
                           strings.tooltip.remove_layer.c_str(),
                           has_active_layer)) {
    model.enqueue<RemoveLayerEvent>(map.active_layer);
  }

  if (ui::push_icon_button(TAC_ICON_DUPLICATE,
                           strings.tooltip.duplicate_layer.c_str(),
                           has_active_layer)) {
    model.enqueue<DuplicateLayerEvent>(map.active_layer);
  }

  if (ui::push_icon_button(
          TAC_ICON_MOVE_UP,
          strings.tooltip.move_layer_up.c_str(),
          has_active_layer &&
              sys::can_move_layer_up(registry, root_layer, map.active_layer))) {
    model.enqueue<MoveLayerUpEvent>(map.active_layer);
  }

  if (ui::push_icon_button(
          TAC_ICON_MOVE_DOWN,
          strings.tooltip.move_layer_down.c_str(),
          has_active_layer &&
              sys::can_move_layer_down(registry, root_layer, map.active_layer))) {
    model.enqueue<MoveLayerDownEvent>(map.active_layer);
  }
}

void _push_contents(ModelView& model, const Strings& strings, LayerDockState& state)
{
  _push_side_buttons(model, strings, state);

  ImGui::SameLine();
  const ui::Group group;

  auto& dispatcher = model.get_dispatcher();
  const auto& registry = model.get_registry();

  const auto document_entity = sys::get_active_document(registry);
  const auto& map_document = registry.get<MapDocument>(document_entity);

  const auto& map = registry.get<Map>(map_document.map);
  const auto& root_layer = registry.get<GroupLayer>(map.root_layer);

  if (root_layer.children.empty()) {
    ui::prepare_vertical_alignment_center(1);
    ui::push_centered_label(strings.misc.map_has_no_layers.c_str());
  }
  else {
    const ImVec2 size {-kMinFloat, -kMinFloat};
    if (const ui::ListBox list {"##LayerTreeNode", size}; list.is_open()) {
      for (const auto layer_entity: root_layer.children) {
        ui::layer_selectable(registry, map_document.map, layer_entity, dispatcher);
      }
    }
  }

  push_rename_layer_dialog(registry, state.rename_layer_dialog, dispatcher);
}

}  // namespace

void push_layer_dock_widget(ModelView& model, LayerDockState& state)
{
  TACTILE_ASSERT(model.has_active_document());

  const auto& settings = model.get_settings();
  if (!settings.test_flag(SETTINGS_SHOW_LAYER_DOCK_BIT)) {
    return;
  }

  const auto& strings = model.get_language_strings();

  bool show_layer_dock = true;
  const ui::Window dock {strings.window.layer_dock.c_str(),
                         ImGuiWindowFlags_NoCollapse,
                         &show_layer_dock};

  if (show_layer_dock != settings.test_flag(SETTINGS_SHOW_LAYER_DOCK_BIT)) {
    model.enqueue<SetFlagSettingEvent>(SETTINGS_SHOW_LAYER_DOCK_BIT, show_layer_dock);
  }

  state.has_focus = dock.has_focus(ImGuiFocusedFlags_RootAndChildWindows);

  if (dock.is_open()) {
    _push_contents(model, strings, state);
  }
}

auto is_layer_dock_enabled(const Registry& registry) -> bool
{
  return sys::is_map_document_active(registry);
}

}  // namespace tactile
