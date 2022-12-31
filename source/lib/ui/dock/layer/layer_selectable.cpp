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
#include <fmt/format.h>
#include <imgui.h>

#include "common/type/string.hpp"
#include "common/util/fmt.hpp"
#include "core/debug/assert.hpp"
#include "core/layer/group_layer.hpp"
#include "core/layer/object_layer.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/document/map_document.hpp"
#include "model/event/layer_events.hpp"
#include "model/event/object_events.hpp"
#include "model/event/property_events.hpp"
#include "ui/style/icons.hpp"
#include "ui/widget/scoped.hpp"

namespace tactile::ui {
namespace {

constexpr int base_node_flags =
    ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick |
    ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth;

void update_layer_popup(const Map& map, const Layer& layer, entt::dispatcher& dispatcher)
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

    const auto& root = map.invisible_root();

    ImGui::Separator();
    if (ImGui::MenuItem(lang.action.move_layer_up.c_str(),
                        nullptr,
                        false,
                        root.can_move_layer_up(layer.get_uuid()))) {
      dispatcher.enqueue<MoveLayerUpEvent>(layer.get_uuid());
    }

    if (ImGui::MenuItem(lang.action.move_layer_down.c_str(),
                        nullptr,
                        false,
                        root.can_move_layer_down(layer.get_uuid()))) {
      dispatcher.enqueue<MoveLayerDownEvent>(layer.get_uuid());
    }
  }
}

void show_object_selectable(const ObjectLayer& layer,
                            const Object& object,
                            entt::dispatcher& dispatcher)
{
  const auto object_id = object.get_uuid();
  const auto object_type = object.get_type();

  const Scope scope {object_id};

  const auto& lang = get_current_language();
  const auto* icon = get_icon(object_type);

  String name;
  if (object.get_ctx().name().empty()) {
    TACTILE_ASSERT(object.get_meta_id().has_value());
    name = fmt::format("{} Object {}", icon, object.get_meta_id().value());
  }
  else {
    name = fmt::format("{} {}", icon, object.get_ctx().name());
  }

  if (Selectable::ListItem(name.c_str(), layer.active_object_id() == object_id)) {
    dispatcher.enqueue<SelectObjectEvent>(layer.get_uuid(), object_id);
  }

  if (ImGui::IsItemActivated() ||
      (ImGui::IsItemHovered() && ImGui::IsItemClicked(ImGuiMouseButton_Right))) {
    dispatcher.enqueue<SelectObjectEvent>(layer.get_uuid(), object_id);
  }

  if (auto popup = Popup::for_item("##ObjectPopup"); popup.is_open()) {
    if (ImGui::MenuItem(lang.action.inspect_object.c_str())) {
      dispatcher.enqueue<InspectContextEvent>(object_id);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(lang.action.toggle_object_visibility.c_str())) {
      dispatcher.enqueue<SetObjectVisibleEvent>(object_id, !object.is_visible());
    }

    ImGui::Separator();

    if (ImGui::MenuItem(lang.action.remove_object.c_str())) {
      dispatcher.enqueue<RemoveObjectEvent>(object_id);
    }
  }
}

void show_object_layer_selectable(const Map& map,
                                  const ObjectLayer& layer,
                                  const ImGuiTreeNodeFlags flags,
                                  entt::dispatcher& dispatcher)
{
  const FmtString<256> name {"{} {}", TAC_ICON_OBJECT_LAYER, layer.get_ctx().name()};
  if (TreeNode tree_node {"##ObjectLayerTree", flags, name.data()}; tree_node.is_open()) {
    Indent indent;

    if (ImGui::IsItemActivated() ||
        (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))) {
      dispatcher.enqueue<SelectLayerEvent>(layer.get_uuid());
    }

    update_layer_popup(map, layer, dispatcher);

    const auto& object_layer = dynamic_cast<const ObjectLayer&>(layer);
    for (const auto& [object_id, object]: object_layer) {
      show_object_selectable(object_layer, *object, dispatcher);
    }
  }
  else {
    if (ImGui::IsItemActivated() ||
        (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))) {
      dispatcher.enqueue<SelectLayerEvent>(layer.get_uuid());
    }

    update_layer_popup(map, layer, dispatcher);
  }
}

void show_group_layer_selectable(const MapDocument& document,
                                 const Layer& layer,
                                 const ImGuiTreeNodeFlags flags,
                                 entt::dispatcher& dispatcher)
{
  const auto& map = document.get_map();

  const FmtString<256> name {"{} {}", TAC_ICON_GROUP_LAYER, layer.get_ctx().name()};
  if (TreeNode tree_node {"##GroupLayerTreeNode", flags, name.data()};
      tree_node.is_open()) {
    Indent indent;

    if (ImGui::IsItemActivated() ||
        (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))) {
      dispatcher.enqueue<SelectLayerEvent>(layer.get_uuid());
    }

    update_layer_popup(map, layer, dispatcher);

    const auto& group = dynamic_cast<const GroupLayer&>(layer);
    for (const auto& child: group.get_storage()) {
      layer_selectable(document, *child, dispatcher);
    }
  }
  else {
    if (ImGui::IsItemActivated() ||
        (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))) {
      dispatcher.enqueue<SelectLayerEvent>(layer.get_uuid());
    }

    update_layer_popup(map, layer, dispatcher);
  }
}

}  // namespace

void layer_selectable(const MapDocument& document,
                      const Layer& layer,
                      entt::dispatcher& dispatcher)
{
  const auto& map = document.get_map();

  const Scope scope {layer.get_uuid()};

  const auto is_active_layer = map.active_layer_id() == layer.get_uuid();
  const auto flags =
      is_active_layer ? (base_node_flags | ImGuiTreeNodeFlags_Selected) : base_node_flags;

  const FmtString name {"{} {}", get_icon(layer.get_type()), layer.get_ctx().name()};

  switch (layer.get_type()) {
    case LayerType::TileLayer: {
      if (Selectable::ListItem(name.data(), is_active_layer)) {
        dispatcher.enqueue<SelectLayerEvent>(layer.get_uuid());
      }

      // Make sure to select the layer item when right-clicked as well
      if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
        dispatcher.enqueue<SelectLayerEvent>(layer.get_uuid());
      }

      update_layer_popup(map, layer, dispatcher);
      break;
    }
    case LayerType::ObjectLayer: {
      const auto& object_layer = dynamic_cast<const ObjectLayer&>(layer);
      show_object_layer_selectable(map, object_layer, flags, dispatcher);
      break;
    }
    case LayerType::GroupLayer: {
      show_group_layer_selectable(document,
                                  layer,
                                  ImGuiTreeNodeFlags_DefaultOpen | flags,
                                  dispatcher);
      break;
    }
  }
}

}  // namespace tactile::ui
