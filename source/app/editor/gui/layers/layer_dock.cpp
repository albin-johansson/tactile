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

#include <optional>  // optional

#include <entt/entt.hpp>
#include <imgui.h>

#include "add_layer_context_menu.hpp"
#include "core/components/attributes.hpp"
#include "core/components/layers.hpp"
#include "core/components/parent.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/layer_tree_system.hpp"
#include "core/systems/registry_system.hpp"
#include "dialogs/rename_layer_dialog.hpp"
#include "editor/constants.hpp"
#include "editor/events/layer_events.hpp"
#include "editor/gui/alignment.hpp"
#include "editor/gui/common/button.hpp"
#include "editor/gui/common/centered_text.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/layers/views/layer_item.hpp"
#include "editor/gui/scoped.hpp"
#include "editor/model.hpp"
#include "io/persistence/preferences.hpp"
#include "misc/assert.hpp"

namespace tactile {
namespace {

inline RenameLayerDialog _rename_layer_dialog;
inline AddLayerContextMenu _add_layer_context_menu;
inline std::optional<LayerID> _rename_target_id;
constinit bool _is_focused = false;

void _update_side_buttons(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  const auto& registry = model.get_active_registry();
  const auto activeLayerEntity = registry.ctx<comp::ActiveLayer>().entity;
  const auto hasActiveLayer = activeLayerEntity != entt::null;

  std::optional<LayerID> activeLayerId;
  if (hasActiveLayer) {
    const auto& layer = sys::checked_get<comp::Layer>(registry, activeLayerEntity);
    activeLayerId = layer.id;
  }

  scoped::Group group;

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

  if (icon_button(
          TAC_ICON_MOVE_UP,
          "Move layer up",
          hasActiveLayer && sys::can_move_layer_up(registry, activeLayerEntity))) {
    dispatcher.enqueue<MoveLayerUpEvent>(activeLayerId.value());
  }

  if (icon_button(
          TAC_ICON_MOVE_DOWN,
          "Move layer down",
          hasActiveLayer && sys::can_move_layer_down(registry, activeLayerEntity))) {
    dispatcher.enqueue<MoveLayerDownEvent>(activeLayerId.value());
  }
}

void _update_rename_dialog(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  if (_rename_target_id.has_value()) {
    const auto targetLayerId = *_rename_target_id;

    const auto& registry = model.get_active_registry();
    const auto entity = sys::find_layer(registry, targetLayerId);

    TACTILE_ASSERT(entity != entt::null);
    const auto& context = sys::checked_get<comp::AttributeContext>(registry, entity);

    _rename_layer_dialog.show(targetLayerId, context.name);
    _rename_target_id.reset();
  }

  _rename_layer_dialog.update(model, dispatcher);
}

void _update_contents(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  _update_side_buttons(model, dispatcher);

  ImGui::SameLine();
  scoped::Group group;

  const auto& registry = model.get_active_registry();
  if (registry.view<comp::Layer>().empty()) {
    prepare_vertical_alignment_center(1);
    centered_text("This map has no layers!");
  }
  else {
    const ImVec2 size{-min_float, -min_float};
    if (scoped::ListBox list{"##LayerTreeNode", size}; list.is_open()) {
      /* Note, we rely on the LayerTreeNode pool being sorted, so we can't include
         other components in the view query directly. */
      for (auto&& [entity, _] : registry.view<comp::LayerTreeNode>().each()) {
        const auto& parent = sys::checked_get<comp::Parent>(registry, entity);
        if (parent.entity == entt::null) {
          layer_item_view(registry, dispatcher, entity);
        }
      }
    }
  }

  _update_rename_dialog(model, dispatcher);
}

}  // namespace

void update_layer_dock(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  TACTILE_ASSERT(model.has_active_document());

  auto& prefs = get_preferences();
  bool visible = prefs.is_layer_dock_visible();

  if (!visible) {
    return;
  }

  constexpr auto flags = ImGuiWindowFlags_NoCollapse;

  scoped::Window dock{"Layers", flags, &visible};
  _is_focused = dock.is_open() &&  //
                ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);

  if (dock.is_open()) {
    _update_contents(model, dispatcher);
  }

  prefs.set_layer_dock_visible(visible);
}

void show_rename_layer_dialog(const LayerID layerId)
{
  _rename_target_id = layerId;
}

auto is_layer_dock_focused() -> bool
{
  return _is_focused;
}

}  // namespace tactile
