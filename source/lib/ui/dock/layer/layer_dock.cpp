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
#include "core/context.hpp"
#include "core/map.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/document.hpp"
#include "model/event/layer_events.hpp"
#include "model/model.hpp"
#include "model/settings.hpp"
#include "model/systems/document_system.hpp"
#include "model/systems/group_layer_system.hpp"
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

void _update_side_buttons(const Model& model, entt::dispatcher& dispatcher)
{
  const auto& lang = get_current_language();

  const auto& document_entity = sys::get_active_document(model);
  const auto& map_document = model.get<MapDocument>(document_entity);
  const auto& map = model.get<Map>(map_document.map);
  const auto& root_layer = model.get<GroupLayer>(map.root_layer);

  const auto has_active_layer = map.active_layer != kNullEntity;
  const Group group;

  if (ui_icon_button(TAC_ICON_ADD, lang.tooltip.add_new_layer.c_str())) {
    gDockState.add_layer_context_menu.show();
  }

  gDockState.add_layer_context_menu.update(model, dispatcher);

  if (ui_icon_button(TAC_ICON_REMOVE,
                     lang.tooltip.remove_layer.c_str(),
                     has_active_layer)) {
    dispatcher.enqueue<RemoveLayerEvent>(map.active_layer);
  }

  if (ui_icon_button(TAC_ICON_DUPLICATE,
                     lang.tooltip.duplicate_layer.c_str(),
                     has_active_layer)) {
    dispatcher.enqueue<DuplicateLayerEvent>(map.active_layer);
  }

  if (ui_icon_button(TAC_ICON_MOVE_UP,
                     lang.tooltip.move_layer_up.c_str(),
                     has_active_layer &&
                         sys::can_move_layer_up(model, root_layer, map.active_layer))) {
    dispatcher.enqueue<MoveLayerUpEvent>(map.active_layer);
  }

  if (ui_icon_button(TAC_ICON_MOVE_DOWN,
                     lang.tooltip.move_layer_down.c_str(),
                     has_active_layer &&
                         sys::can_move_layer_down(model, root_layer, map.active_layer))) {
    dispatcher.enqueue<MoveLayerDownEvent>(map.active_layer);
  }
}

void _update_rename_dialog(const Model& model, entt::dispatcher& dispatcher)
{
  if (gDockState.rename_target_layer_entity.has_value()) {
    const auto target_layer_entity = *gDockState.rename_target_layer_entity;
    const auto& layer_context = model.get<Context>(target_layer_entity);

    open_rename_layer_dialog(target_layer_entity, layer_context.name);
    gDockState.rename_target_layer_entity.reset();
  }

  update_rename_layer_dialog(dispatcher);
}

void _update_contents(const Model& model, entt::dispatcher& dispatcher)
{
  _update_side_buttons(model, dispatcher);

  ImGui::SameLine();
  const Group group;

  const auto document_entity = sys::get_active_document(model);
  const auto& map_document = model.get<MapDocument>(document_entity);

  const auto& map = model.get<Map>(map_document.map);
  const auto& root_layer = model.get<GroupLayer>(map.root_layer);

  if (root_layer.children.empty()) {
    const auto& lang = get_current_language();

    prepare_vertical_alignment_center(1);
    ui_centered_label(lang.misc.map_has_no_layers.c_str());
  }
  else {
    const ImVec2 size {-kMinFloat, -kMinFloat};
    if (const ListBox list {"##LayerTreeNode", size}; list.is_open()) {
      for (const auto layer_entity: root_layer.children) {
        layer_selectable(model, map_document.map, layer_entity, dispatcher);
      }
    }
  }

  _update_rename_dialog(model, dispatcher);
}

}  // namespace

void show_layer_dock(const Model& model, Entity, Dispatcher& dispatcher)
{
  TACTILE_ASSERT(sys::has_active_document(model));

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
    _update_contents(model, dispatcher);
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
