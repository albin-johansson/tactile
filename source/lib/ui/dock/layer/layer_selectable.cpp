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

#include "layer_selectable.hpp"

#include <fmt/format.h>
#include <imgui.h>

#include "common/debug/assert.hpp"
#include "common/fmt/fmt_string.hpp"
#include "common/type/string.hpp"
#include "model/contexts/context_components.hpp"
#include "model/events/layer_events.hpp"
#include "model/events/object_events.hpp"
#include "model/events/property_events.hpp"
#include "model/i18n/language_system.hpp"
#include "model/layers/layer_components.hpp"
#include "model/layers/layer_tree_system.hpp"
#include "model/maps/map_components.hpp"
#include "model/objects/object_components.hpp"
#include "ui/style/icons.hpp"
#include "ui/widget/scoped.hpp"

namespace tactile::ui {
namespace {

constexpr int kBaseNodeFlags = ImGuiTreeNodeFlags_OpenOnArrow |
                               ImGuiTreeNodeFlags_OpenOnDoubleClick |
                               ImGuiTreeNodeFlags_SpanAvailWidth;

void _push_layer_popup(const Registry& registry,
                       const Strings& strings,
                       const Entity map_entity,
                       const Entity layer_entity,
                       Dispatcher& dispatcher)
{
  if (auto popup = Popup::for_item("##LayerPopup"); popup.is_open()) {
    const auto& layer = registry.get<Layer>(layer_entity);
    const auto& map = registry.get<Map>(map_entity);
    const auto& root_layer = registry.get<GroupLayer>(map.root_layer);

    if (ImGui::MenuItem(strings.action.inspect_layer.c_str())) {
      dispatcher.enqueue<InspectContextEvent>(layer_entity);
    }

    ImGui::Separator();
    if (ImGui::MenuItem(strings.action.rename_layer.c_str())) {
      dispatcher.enqueue<ShowRenameLayerDialogEvent>(layer_entity);
    }

    ImGui::Separator();
    if (ImGui::MenuItem(strings.action.duplicate_layer.c_str())) {
      dispatcher.enqueue<DuplicateLayerEvent>(layer_entity);
    }

    ImGui::Separator();
    if (ImGui::MenuItem(strings.action.remove_layer.c_str())) {
      dispatcher.enqueue<RemoveLayerEvent>(layer_entity);
    }

    ImGui::Separator();
    if (ImGui::MenuItem(strings.action.toggle_layer_visible.c_str(),
                        nullptr,
                        layer.visible)) {
      dispatcher.enqueue<SetLayerVisibleEvent>(layer_entity, !layer.visible);
    }

    if (float opacity = layer.opacity;
        ImGui::SliderFloat(strings.misc.opacity.c_str(), &opacity, 0, 1.0f)) {
      dispatcher.enqueue<SetLayerOpacityEvent>(layer_entity, opacity);
    }

    ImGui::Separator();
    if (ImGui::MenuItem(strings.action.move_layer_up.c_str(),
                        nullptr,
                        false,
                        sys::can_move_layer_up(registry, root_layer, layer_entity))) {
      dispatcher.enqueue<MoveLayerUpEvent>(layer_entity);
    }

    if (ImGui::MenuItem(strings.action.move_layer_down.c_str(),
                        nullptr,
                        false,
                        sys::can_move_layer_down(registry, root_layer, layer_entity))) {
      dispatcher.enqueue<MoveLayerDownEvent>(layer_entity);
    }
  }
}

void _push_object_selectable(const Registry& registry,
                             const Strings& strings,
                             const Entity layer_entity,
                             const Entity object_entity,
                             Dispatcher& dispatcher)
{
  const auto& object_layer = registry.get<ObjectLayer>(layer_entity);

  const auto& object = registry.get<Object>(object_entity);
  const auto& object_context = registry.get<Context>(object_entity);

  const Scope scope {object_entity};

  const auto* icon = get_icon(object.type);

  String name;
  if (object_context.name.empty()) {
    TACTILE_ASSERT(object.meta_id.has_value());
    name = fmt::format("{} Object {}", icon, object.meta_id.value());
  }
  else {
    name = fmt::format("{} {}", icon, object_context.name);
  }

  if (Selectable::list_item(name.c_str(), object_layer.active_object == object_entity)) {
    dispatcher.enqueue<SelectObjectEvent>(layer_entity, object_entity);
  }

  if (ImGui::IsItemActivated() ||
      (ImGui::IsItemHovered() && ImGui::IsItemClicked(ImGuiMouseButton_Right))) {
    dispatcher.enqueue<SelectObjectEvent>(layer_entity, object_entity);
  }

  if (auto popup = Popup::for_item("##ObjectPopup"); popup.is_open()) {
    if (ImGui::MenuItem(strings.action.inspect_object.c_str())) {
      dispatcher.enqueue<InspectContextEvent>(object_entity);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(strings.action.toggle_object_visibility.c_str())) {
      dispatcher.enqueue<SetObjectVisibleEvent>(object_entity, !object.visible);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(strings.action.remove_object.c_str())) {
      dispatcher.enqueue<RemoveObjectEvent>(object_entity);
    }
  }
}

void _push_object_layer_selectable(const Registry& registry,
                                   const Strings& strings,
                                   const Entity map_entity,
                                   const Entity layer_entity,
                                   const ImGuiTreeNodeFlags flags,
                                   Dispatcher& dispatcher)
{
  const auto& layer_context = registry.get<Context>(layer_entity);

  const FmtString<256> name {"{} {}", TAC_ICON_OBJECT_LAYER, layer_context.name};
  if (const TreeNode tree_node {"##ObjectLayerTree", flags, name.data()};
      tree_node.is_open()) {
    const Indent indent;

    if (ImGui::IsItemActivated() ||
        (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))) {
      dispatcher.enqueue<SelectLayerEvent>(layer_entity);
    }

    _push_layer_popup(registry, strings, map_entity, layer_entity, dispatcher);

    const auto& object_layer = registry.get<ObjectLayer>(layer_entity);
    for (const auto object_entity: object_layer.objects) {
      _push_object_selectable(registry, strings, layer_entity, object_entity, dispatcher);
    }
  }
  else {
    if (ImGui::IsItemActivated() ||
        (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))) {
      dispatcher.enqueue<SelectLayerEvent>(layer_entity);
    }

    _push_layer_popup(registry, strings, map_entity, layer_entity, dispatcher);
  }
}

void _push_group_layer_selectable(const Registry& registry,
                                  const Strings& strings,
                                  const Entity map_entity,
                                  const Entity layer_entity,
                                  const ImGuiTreeNodeFlags flags,
                                  entt::dispatcher& dispatcher)
{
  const auto& layer_context = registry.get<Context>(layer_entity);

  const FmtString<256> name {"{} {}", TAC_ICON_GROUP_LAYER, layer_context.name};
  if (const TreeNode tree_node {"##GroupLayerTreeNode", flags, name.data()};
      tree_node.is_open()) {
    const Indent indent;

    if (ImGui::IsItemActivated() ||
        (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))) {
      dispatcher.enqueue<SelectLayerEvent>(layer_entity);
    }

    _push_layer_popup(registry, strings, map_entity, layer_entity, dispatcher);

    const auto& group_layer = registry.get<GroupLayer>(layer_entity);
    for (const auto& child_layer_entity: group_layer.children) {
      layer_selectable(registry, map_entity, child_layer_entity, dispatcher);
    }
  }
  else {
    if (ImGui::IsItemActivated() ||
        (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))) {
      dispatcher.enqueue<SelectLayerEvent>(layer_entity);
    }

    _push_layer_popup(registry, strings, map_entity, layer_entity, dispatcher);
  }
}

}  // namespace

void layer_selectable(const Registry& registry,
                      const Entity map_entity,
                      const Entity layer_entity,
                      Dispatcher& dispatcher)
{
  const Scope scope {layer_entity};

  const auto& strings = sys::get_current_language_strings(registry);

  const auto& map = registry.get<Map>(map_entity);
  const auto& layer = registry.get<Layer>(layer_entity);
  const auto& layer_context = registry.get<Context>(layer_entity);

  const auto is_layer_active = map.active_layer == layer_entity;
  const auto flags = is_layer_active ? (kBaseNodeFlags | ImGuiTreeNodeFlags_Selected)  //
                                     : kBaseNodeFlags;

  const FmtString name {"{} {}", get_icon(layer.type), layer_context.name};

  switch (layer.type) {
    case LayerType::TileLayer: {
      if (Selectable::list_item(name.data(), is_layer_active)) {
        dispatcher.enqueue<SelectLayerEvent>(layer_entity);
      }

      // Make sure to select the layer item when right-clicked as well
      if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
        dispatcher.enqueue<SelectLayerEvent>(layer_entity);
      }

      _push_layer_popup(registry, strings, map_entity, layer_entity, dispatcher);
      break;
    }
    case LayerType::ObjectLayer: {
      _push_object_layer_selectable(registry,
                                    strings,
                                    map_entity,
                                    layer_entity,
                                    flags,
                                    dispatcher);
      break;
    }
    case LayerType::GroupLayer: {
      _push_group_layer_selectable(registry,
                                   strings,
                                   map_entity,
                                   layer_entity,
                                   ImGuiTreeNodeFlags_DefaultOpen | flags,
                                   dispatcher);
      break;
    }
  }
}

}  // namespace tactile::ui
