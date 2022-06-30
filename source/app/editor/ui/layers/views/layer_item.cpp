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

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>

#include "core/documents/map_document.hpp"
#include "core/events/layer_events.hpp"
#include "core/events/property_events.hpp"
#include "core/utils/formatted_string.hpp"
#include "editor/ui/icons.hpp"
#include "editor/ui/scoped.hpp"
#include "misc/panic.hpp"

namespace tactile::ui {
namespace {

constexpr int _base_node_flags =
    ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow |
    ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth |
    ImGuiTreeNodeFlags_SpanFullWidth;

[[nodiscard]] auto _get_icon(const LayerType type) -> const char*
{
  switch (type) {
    case LayerType::TileLayer:
      return TAC_ICON_TILE_LAYER;

    case LayerType::ObjectLayer:
      return TAC_ICON_OBJECT_LAYER;

    case LayerType::GroupLayer:
      return TAC_ICON_GROUP_LAYER;

    default:
      throw TactileError("Failed to recognize layer type!");
  }
}

void _update_layer_item_popup(const core::Map&    map,
                              const core::ILayer& layer,
                              entt::dispatcher&   dispatcher)
{
  if (auto popup = Popup::for_item("##LayerItemPopup"); popup.is_open()) {
    if (ImGui::MenuItem(TAC_ICON_INSPECT " Inspect Layer")) {
      dispatcher.enqueue<InspectContextEvent>(layer.get_uuid());
    }

    ImGui::Separator();
    if (ImGui::MenuItem(TAC_ICON_EDIT " Rename Layer")) {
      dispatcher.enqueue<OpenRenameLayerDialogEvent>(layer.get_uuid());
    }

    ImGui::Separator();
    if (ImGui::MenuItem(TAC_ICON_DUPLICATE " Duplicate Layer")) {
      dispatcher.enqueue<DuplicateLayerEvent>(layer.get_uuid());
    }

    ImGui::Separator();
    if (ImGui::MenuItem(TAC_ICON_REMOVE " Remove Layer")) {
      dispatcher.enqueue<RemoveLayerEvent>(layer.get_uuid());
    }

    ImGui::Separator();
    if (ImGui::MenuItem(TAC_ICON_VISIBILITY " Toggle Layer Visibility",
                        nullptr,
                        layer.is_visible())) {
      dispatcher.enqueue<SetLayerVisibleEvent>(layer.get_uuid(), !layer.is_visible());
    }

    if (auto opacity = layer.get_opacity();
        ImGui::SliderFloat("Opacity", &opacity, 0, 1.0f)) {
      dispatcher.enqueue<SetLayerOpacityEvent>(layer.get_uuid(), opacity);
    }

    ImGui::Separator();
    if (ImGui::MenuItem(TAC_ICON_MOVE_UP " Move Layer Up",
                        nullptr,
                        false,
                        map.can_move_layer_up(layer.get_uuid()))) {
      dispatcher.enqueue<MoveLayerUpEvent>(layer.get_uuid());
    }

    if (ImGui::MenuItem(TAC_ICON_MOVE_DOWN " Move Layer Down",
                        nullptr,
                        false,
                        map.can_move_layer_down(layer.get_uuid()))) {
      dispatcher.enqueue<MoveLayerDownEvent>(layer.get_uuid());
    }
  }
}

void _show_group_layer_item(const MapDocument&       document,
                            const core::ILayer&      layer,
                            const ImGuiTreeNodeFlags flags,
                            entt::dispatcher&        dispatcher)
{
  const auto& map = document.get_map();

  ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());
  if (TreeNode treeNode{"##GroupLayerTreeNode", flags, "%s", layer.get_name().c_str()};
      treeNode.is_open()) {
    ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());

    if (ImGui::IsItemActivated() ||
        (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))) {
      dispatcher.enqueue<SelectLayerEvent>(layer.get_uuid());
    }

    _update_layer_item_popup(map, layer, dispatcher);

    const auto& group = dynamic_cast<const core::GroupLayer&>(layer);
    for (const auto& child : group.storage()) {
      layer_item_view(document, *child.get(), dispatcher);
    }
  }
  else {
    ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());

    if (ImGui::IsItemActivated() ||
        (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))) {
      dispatcher.enqueue<SelectLayerEvent>(layer.get_uuid());
    }

    _update_layer_item_popup(map, layer, dispatcher);
  }
}

}  // namespace

void layer_item_view(const MapDocument&  document,
                     const core::ILayer& layer,
                     entt::dispatcher&   dispatcher)
{
  const auto& map = document.get_map();

  const Scope scope{layer.get_uuid()};

  const auto isActiveLayer = map.active_layer_id() == layer.get_uuid();
  const auto flags = isActiveLayer ? (_base_node_flags | ImGuiTreeNodeFlags_Selected)  //
                                   : _base_node_flags;

  const FormattedString name{"{} {}", _get_icon(layer.get_type()), layer.get_name()};

  if (layer.get_type() != LayerType::GroupLayer) {
    if (ImGui::Selectable(name.data(), isActiveLayer)) {
      dispatcher.enqueue<SelectLayerEvent>(layer.get_uuid());
    }

    /* Make sure to select the layer item when right-clicked as well */
    if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
      dispatcher.enqueue<SelectLayerEvent>(layer.get_uuid());
    }

    _update_layer_item_popup(map, layer, dispatcher);
  }
  else {
    _show_group_layer_item(document, layer, flags, dispatcher);
  }
}

}  // namespace tactile::ui
