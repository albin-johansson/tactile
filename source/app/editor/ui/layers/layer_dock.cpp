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
#include "core/documents/map_document.hpp"
#include "core/events/layer_events.hpp"
#include "core/model.hpp"
#include "editor/constants.hpp"
#include "editor/ui/alignment.hpp"
#include "editor/ui/common/button.hpp"
#include "editor/ui/common/centered_text.hpp"
#include "editor/ui/icons.hpp"
#include "editor/ui/layers/add_layer_context_menu.hpp"
#include "editor/ui/layers/views/layer_item.hpp"
#include "editor/ui/scoped.hpp"
#include "editor/ui/shared/dialog_state.hpp"
#include "editor/ui/shared/dialogs.hpp"
#include "io/persistence/preferences.hpp"
#include "misc/assert.hpp"

namespace tactile::ui {
namespace {

inline AddLayerContextMenu _add_layer_context_menu;
inline Maybe<UUID>         _rename_target_id;
constinit bool             _is_focused = false;

void _update_side_buttons(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  const auto& document = model.require_active_map();
  const auto& map = document.get_map();

  const auto activeLayerId = map.active_layer_id();
  const auto hasActiveLayer = activeLayerId.has_value();

  Group group;

  if (icon_button(TAC_ICON_ADD, "Add new layer")) {
    _add_layer_context_menu.show();
  }

  _add_layer_context_menu.update(model, dispatcher);

  if (icon_button(TAC_ICON_REMOVE, "Remove layer", hasActiveLayer)) {
    dispatcher.enqueue<RemoveLayerEvent>(activeLayerId.value());
  }

  if (icon_button(TAC_ICON_DUPLICATE, "Duplicate layer", hasActiveLayer)) {
    dispatcher.enqueue<DuplicateLayerEvent>(activeLayerId.value());
  }

  if (icon_button(TAC_ICON_MOVE_UP,
                  "Move layer up",
                  hasActiveLayer && map.can_move_layer_up(*activeLayerId))) {
    dispatcher.enqueue<MoveLayerUpEvent>(activeLayerId.value());
  }

  if (icon_button(TAC_ICON_MOVE_DOWN,
                  "Move layer down",
                  hasActiveLayer && map.can_move_layer_down(*activeLayerId))) {
    dispatcher.enqueue<MoveLayerDownEvent>(activeLayerId.value());
  }
}

void _update_rename_dialog(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  auto& dialogs = get_dialogs();

  if (_rename_target_id.has_value()) {
    const auto targetLayerId = *_rename_target_id;

    const auto& document = model.require_active_map();
    const auto& map = document.get_map();
    const auto& layer = map.view_layer(targetLayerId);

    dialogs.rename_layer.show(targetLayerId, layer.get_name());
    _rename_target_id.reset();
  }

  dialogs.rename_layer.update(model, dispatcher);
}

void _update_contents(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  _update_side_buttons(model, dispatcher);

  ImGui::SameLine();
  Group group;

  const auto& document = model.require_active_map();
  const auto& map = document.get_map();

  if (map.layer_count() == 0) {
    prepare_vertical_alignment_center(1);
    centered_text("This map has no layers!");
  }
  else {
    const ImVec2 size{-min_float, -min_float};
    if (ListBox list{"##LayerTreeNode", size}; list.is_open()) {
      map.visit_layers([&](const core::ILayer* layer) {
        if (!layer->get_parent().has_value()) {
          layer_item_view(document, *layer, dispatcher);
        }
      });
    }
  }

  _update_rename_dialog(model, dispatcher);
}

}  // namespace

void update_layer_dock(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  TACTILE_ASSERT(model.has_active_document());

  auto& prefs = io::get_preferences();
  bool  visible = prefs.is_layer_dock_visible();

  if (!visible) {
    return;
  }

  constexpr auto flags = ImGuiWindowFlags_NoCollapse;

  Window dock{"Layers", flags, &visible};
  _is_focused = dock.is_open() &&  //
                ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);

  if (dock.is_open()) {
    _update_contents(model, dispatcher);
  }

  prefs.set_layer_dock_visible(visible);
}

void show_rename_layer_dialog(const UUID& layerId)
{
  _rename_target_id = layerId;
}

auto is_layer_dock_focused() -> bool
{
  return _is_focused;
}

}  // namespace tactile::ui
