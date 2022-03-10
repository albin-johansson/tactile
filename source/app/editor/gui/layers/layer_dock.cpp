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

#include <imgui.h>

#include "add_layer_context_menu.hpp"
#include "core/components/attributes.hpp"
#include "core/components/layers.hpp"
#include "core/components/parent.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/layer_tree_system.hpp"
#include "editor/events/layer_events.hpp"
#include "editor/gui/alignment.hpp"
#include "editor/gui/common/button.hpp"
#include "editor/gui/common/centered_text.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/scoped.hpp"
#include "editor/model.hpp"
#include "io/persistence/preferences.hpp"
#include "layer_item.hpp"
#include "misc/assert.hpp"
#include "tactile.hpp"

namespace tactile {

struct LayerDock::LayerDockData final
{
  RenameLayerDialog mRenameLayerDialog;
  AddLayerContextMenu mAddLayerContextMenu;
  Maybe<layer_id> mRenameTarget;
};

LayerDock::LayerDock()
    : ADockWidget{"Layers", ImGuiWindowFlags_NoCollapse}
    , mData{std::make_unique<LayerDockData>()}
{
  set_focus_flags(ImGuiFocusedFlags_RootAndChildWindows);
}

LayerDock::~LayerDock() noexcept = default;

void LayerDock::show_rename_layer_dialog(const layer_id id)
{
  mData->mRenameTarget = id;
}

void LayerDock::on_update(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  const auto& registry = model.get_active_registry();

  {
    update_buttons(model, registry, dispatcher);
    ImGui::SameLine();

    scoped::Group group;
    if (registry.view<comp::Layer>().empty()) {
      prepare_vertical_alignment_center(1);
      centered_text("No available layers!");
    }
    else {
      const ImVec2 size{-min_float, -min_float};
      if (scoped::ListBox list{"##LayerTreeNode", size}; list.is_open()) {
        for (auto&& [entity, node] : registry.view<comp::LayerTreeNode>().each()) {
          /* Note, we rely on the layer_tree_node pool being sorted, so we can't include
             other components in the view query directly. */
          const auto& parent = registry.get<comp::Parent>(entity);
          if (parent.entity == entt::null) {
            show_layer_item(registry, dispatcher, entity);
          }
        }
      }
    }
  }

  auto& renameTarget = mData->mRenameTarget;
  auto& renameLayerDialog = mData->mRenameLayerDialog;

  if (renameTarget.has_value()) {
    const auto target = *renameTarget;

    const auto entity = sys::find_layer(registry, target);
    TACTILE_ASSERT(entity != entt::null);

    const auto& context = registry.get<comp::AttributeContext>(entity);

    renameLayerDialog.show(target, context.name);
    renameTarget.reset();
  }

  renameLayerDialog.update(model, dispatcher);
}

void LayerDock::set_visible(const bool visible)
{
  auto& prefs = get_preferences();
  prefs.set_layer_dock_visible(visible);
}

auto LayerDock::is_visible() const -> bool
{
  const auto& prefs = get_preferences();
  return prefs.is_layer_dock_visible();
}

void LayerDock::update_buttons(const DocumentModel& model,
                               const entt::registry& registry,
                               entt::dispatcher& dispatcher)
{
  const auto activeLayerEntity = registry.ctx<comp::ActiveLayer>().entity;
  const auto hasActiveLayer = activeLayerEntity != entt::null;

  Maybe<layer_id> activeLayerId;
  if (hasActiveLayer) {
    const auto& layer = registry.get<comp::Layer>(activeLayerEntity);
    activeLayerId = layer.id;
  }

  scoped::Group group;

  if (button(TAC_ICON_ADD, "Add new layer")) {
    mData->mAddLayerContextMenu.show();
  }

  mData->mAddLayerContextMenu.update(model, dispatcher);

  if (button(TAC_ICON_REMOVE, "Remove layer", hasActiveLayer)) {
    dispatcher.enqueue<RemoveLayerEvent>(*activeLayerId);
  }

  if (button(TAC_ICON_DUPLICATE, "Duplicate layer", hasActiveLayer)) {
    dispatcher.enqueue<DuplicateLayerEvent>(*activeLayerId);
  }

  if (button(TAC_ICON_MOVE_UP,
             "Move layer up",
             hasActiveLayer && sys::can_move_layer_up(registry, activeLayerEntity))) {
    dispatcher.enqueue<MoveLayerUpEvent>(*activeLayerId);
  }

  if (button(TAC_ICON_MOVE_DOWN,
             "Move layer down",
             hasActiveLayer && sys::can_move_layer_down(registry, activeLayerEntity))) {
    dispatcher.enqueue<MoveLayerDownEvent>(*activeLayerId);
  }
}

}  // namespace tactile
