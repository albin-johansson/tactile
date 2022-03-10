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

#include "layer_item.hpp"

#include <imgui.h>

#include "core/components/attributes.hpp"
#include "core/components/layers.hpp"
#include "core/components/parent.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/layer_tree_system.hpp"
#include "core/utils/formatted_string.hpp"
#include "editor/events/layer_events.hpp"
#include "editor/events/property_events.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/scoped.hpp"

namespace tactile {
namespace {

constexpr int _base_node_flags =
    ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow |
    ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth |
    ImGuiTreeNodeFlags_SpanFullWidth;

void _update_layer_item_popup(const entt::registry& registry,
                              entt::dispatcher& dispatcher,
                              const layer_id id)
{
  if (auto popup = scoped::Popup::for_item("##LayerItemPopup"); popup.is_open()) {
    const auto [entity, layer] = sys::get_layer(registry, id);

    if (ImGui::MenuItem(TAC_ICON_INSPECT " Inspect Layer")) {
      dispatcher.enqueue<InspectContextEvent>(entity);
    }

    ImGui::Separator();
    if (ImGui::MenuItem(TAC_ICON_EDIT " Rename Layer")) {
      dispatcher.enqueue<OpenRenameLayerDialogEvent>(id);
    }

    ImGui::Separator();
    if (ImGui::MenuItem(TAC_ICON_DUPLICATE " Duplicate Layer")) {
      dispatcher.enqueue<DuplicateLayerEvent>(id);
    }

    ImGui::Separator();
    if (ImGui::MenuItem(TAC_ICON_REMOVE " Remove Layer")) {
      dispatcher.enqueue<RemoveLayerEvent>(id);
    }

    ImGui::Separator();
    if (ImGui::MenuItem(TAC_ICON_VISIBILITY " Toggle Layer Visibility",
                        nullptr,
                        layer.visible)) {
      dispatcher.enqueue<SetLayerVisibleEvent>(id, !layer.visible);
    }

    if (auto opacity = layer.opacity; ImGui::SliderFloat("Opacity", &opacity, 0, 1.0f)) {
      dispatcher.enqueue<SetLayerOpacityEvent>(id, opacity);
    }

    ImGui::Separator();
    if (ImGui::MenuItem(TAC_ICON_MOVE_UP " Move Layer Up",
                        nullptr,
                        false,
                        sys::can_move_layer_up(registry, entity))) {
      dispatcher.enqueue<MoveLayerUpEvent>(id);
    }

    if (ImGui::MenuItem(TAC_ICON_MOVE_DOWN " Move Layer Down",
                        nullptr,
                        false,
                        sys::can_move_layer_down(registry, entity))) {
      dispatcher.enqueue<MoveLayerDownEvent>(id);
    }
  }
}

void _show_group_layer_item(const entt::registry& registry,
                            entt::dispatcher& dispatcher,
                            const entt::entity layerEntity,
                            const comp::Layer& layer,
                            const ImGuiTreeNodeFlags flags,
                            const char* name)
{
  ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());
  if (scoped::TreeNode treeNode{"##GroupLayerTreeNode", flags, "%s", name};
      treeNode.is_open()) {
    ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());

    if (ImGui::IsItemActivated() ||
        (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))) {
      dispatcher.enqueue<SelectLayerEvent>(layer.id);
    }

    _update_layer_item_popup(registry, dispatcher, layer.id);

    const auto& node = registry.get<comp::LayerTreeNode>(layerEntity);
    for (const auto child : node.children) {
      show_layer_item(registry, dispatcher, child);
    }
  }
  else {
    ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());

    if (ImGui::IsItemActivated() ||
        (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))) {
      dispatcher.enqueue<SelectLayerEvent>(layer.id);
    }

    _update_layer_item_popup(registry, dispatcher, layer.id);
  }
}

}  // namespace

void show_layer_item(const entt::registry& registry,
                     entt::dispatcher& dispatcher,
                     const entt::entity layerEntity)
{
  const auto& layer = registry.get<comp::Layer>(layerEntity);
  const auto& activeLayer = registry.ctx<comp::ActiveLayer>();

  const scoped::Id scope{layer.id};

  const auto isActiveLayer = layerEntity == activeLayer.entity;
  const auto flags = isActiveLayer ? (_base_node_flags | ImGuiTreeNodeFlags_Selected)  //
                                   : _base_node_flags;

  const auto& context = registry.get<comp::AttributeContext>(layerEntity);
  FormattedString name{"{} {}", get_icon(layer.type), context.name};

  if (layer.type != LayerType::group_layer) {
    if (ImGui::Selectable(name.data(), isActiveLayer)) {
      dispatcher.enqueue<SelectLayerEvent>(layer.id);
    }

    /* Make sure to select the layer item when right-clicked as well */
    if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
      dispatcher.enqueue<SelectLayerEvent>(layer.id);
    }

    _update_layer_item_popup(registry, dispatcher, layer.id);
  }
  else {
    _show_group_layer_item(registry, dispatcher, layerEntity, layer, flags, name.data());
  }
}

}  // namespace tactile
