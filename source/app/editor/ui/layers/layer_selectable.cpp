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

#include "layer_selectable.hpp"

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>

#include "core/document/map_document.hpp"
#include "core/event/layer_events.hpp"
#include "core/event/property_events.hpp"
#include "core/util/fmt_string.hpp"
#include "editor/lang/language.hpp"
#include "editor/lang/strings.hpp"
#include "editor/ui/icons.hpp"
#include "editor/ui/scoped.hpp"
#include "misc/panic.hpp"

namespace tactile::ui {
namespace {

constexpr int base_node_flags =
    ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow |
    ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth |
    ImGuiTreeNodeFlags_SpanFullWidth;

[[nodiscard]] auto get_icon(const LayerType type) -> const char*
{
  switch (type) {
    case LayerType::TileLayer:
      return TAC_ICON_TILE_LAYER;

    case LayerType::ObjectLayer:
      return TAC_ICON_OBJECT_LAYER;

    case LayerType::GroupLayer:
      return TAC_ICON_GROUP_LAYER;

    default:
      throw TactileError {"Failed to recognize layer type!"};
  }
}

void update_layer_popup(const Map& map, const ILayer& layer, entt::dispatcher& dispatcher)
{
  const auto& lang = get_current_language();

  if (auto popup = Popup::for_item("##LayerPopup"); popup.is_open()) {
    if (ImGui::MenuItem(lang.action.inspect_layer.c_str())) {
      dispatcher.enqueue<InspectContextEvent>(layer.get_uuid());
    }

    ImGui::Separator();
    if (ImGui::MenuItem(lang.action.rename_layer.c_str())) {
      dispatcher.enqueue<OpenRenameLayerDialogEvent>(layer.get_uuid());
    }

    ImGui::Separator();
    if (ImGui::MenuItem(lang.action.duplicate_layer.c_str())) {
      dispatcher.enqueue<DuplicateLayerEvent>(layer.get_uuid());
    }

    ImGui::Separator();
    if (ImGui::MenuItem(lang.action.remove_layer.c_str())) {
      dispatcher.enqueue<RemoveLayerEvent>(layer.get_uuid());
    }

    ImGui::Separator();
    if (ImGui::MenuItem(lang.action.toggle_layer_visible.c_str(),
                        nullptr,
                        layer.is_visible())) {
      dispatcher.enqueue<SetLayerVisibleEvent>(layer.get_uuid(), !layer.is_visible());
    }

    if (auto opacity = layer.get_opacity();
        ImGui::SliderFloat(lang.misc.opacity.c_str(), &opacity, 0, 1.0f)) {
      dispatcher.enqueue<SetLayerOpacityEvent>(layer.get_uuid(), opacity);
    }

    ImGui::Separator();
    if (ImGui::MenuItem(lang.action.move_layer_up.c_str(),
                        nullptr,
                        false,
                        map.can_move_layer_up(layer.get_uuid()))) {
      dispatcher.enqueue<MoveLayerUpEvent>(layer.get_uuid());
    }

    if (ImGui::MenuItem(lang.action.move_layer_down.c_str(),
                        nullptr,
                        false,
                        map.can_move_layer_down(layer.get_uuid()))) {
      dispatcher.enqueue<MoveLayerDownEvent>(layer.get_uuid());
    }
  }
}

void show_group_layer_selectable(const MapDocument&       document,
                                 const ILayer&            layer,
                                 const ImGuiTreeNodeFlags flags,
                                 entt::dispatcher&        dispatcher)
{
  const auto& map = document.get_map();

  ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());
  if (TreeNode tree_node {"##GroupLayerTreeNode", flags, "%s", layer.get_name().c_str()};
      tree_node.is_open()) {
    ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());

    if (ImGui::IsItemActivated() ||
        (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))) {
      dispatcher.enqueue<SelectLayerEvent>(layer.get_uuid());
    }

    update_layer_popup(map, layer, dispatcher);

    const auto& group = dynamic_cast<const GroupLayer&>(layer);
    for (const auto& child : group.storage()) {
      layer_selectable(document, *child, dispatcher);
    }
  }
  else {
    ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());

    if (ImGui::IsItemActivated() ||
        (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))) {
      dispatcher.enqueue<SelectLayerEvent>(layer.get_uuid());
    }

    update_layer_popup(map, layer, dispatcher);
  }
}

}  // namespace

void layer_selectable(const MapDocument& document,
                      const ILayer&      layer,
                      entt::dispatcher&  dispatcher)
{
  const auto& map = document.get_map();

  const Scope scope {layer.get_uuid()};

  const auto is_active_layer = map.active_layer_id() == layer.get_uuid();
  const auto flags =
      is_active_layer ? (base_node_flags | ImGuiTreeNodeFlags_Selected) : base_node_flags;

  const FmtString name {"{} {}", get_icon(layer.get_type()), layer.get_name()};

  if (layer.get_type() != LayerType::GroupLayer) {
    if (ImGui::Selectable(name.data(), is_active_layer)) {
      dispatcher.enqueue<SelectLayerEvent>(layer.get_uuid());
    }

    /* Make sure to select the layer item when right-clicked as well */
    if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
      dispatcher.enqueue<SelectLayerEvent>(layer.get_uuid());
    }

    update_layer_popup(map, layer, dispatcher);
  }
  else {
    show_group_layer_selectable(document, layer, flags, dispatcher);
  }
}

}  // namespace tactile::ui
