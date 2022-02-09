#include "layer_item.hpp"

#include <imgui.h>

#include "core/components/attribute_context.hpp"
#include "core/components/layer.hpp"
#include "core/components/parent.hpp"
#include "core/utils/formatted_string.hpp"
#include "editor/events/layer_events.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/scoped.hpp"
#include "layer_item_popup.hpp"

namespace tactile {
namespace {

constexpr int _base_node_flags =
    ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow |
    ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth |
    ImGuiTreeNodeFlags_SpanFullWidth;

void _show_group_layer_item(const entt::registry& registry,
                            entt::dispatcher& dispatcher,
                            const entt::entity layerEntity,
                            const comp::layer& layer,
                            const ImGuiTreeNodeFlags flags,
                            const char* name)
{
  ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());
  if (scoped::tree_node treeNode{"##GroupLayerTreeNode", flags, "%s", name};
      treeNode.is_open()) {
    ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());

    if (ImGui::IsItemActivated() ||
        (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))) {
      dispatcher.enqueue<select_layer_event>(layer.id);
    }

    UpdateLayerItemPopup(registry, dispatcher, layer.id);

    const auto& node = registry.get<comp::layer_tree_node>(layerEntity);
    for (const auto child : node.children) {
      show_layer_item(registry, dispatcher, child);
    }
  }
  else {
    ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());

    if (ImGui::IsItemActivated() ||
        (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))) {
      dispatcher.enqueue<select_layer_event>(layer.id);
    }

    UpdateLayerItemPopup(registry, dispatcher, layer.id);
  }
}

}  // namespace

void show_layer_item(const entt::registry& registry,
                     entt::dispatcher& dispatcher,
                     const entt::entity layerEntity)
{
  const auto& layer = registry.get<comp::layer>(layerEntity);
  const auto& activeLayer = registry.ctx<comp::active_layer>();

  const scoped::id scope{layer.id};

  const auto isActiveLayer = layerEntity == activeLayer.entity;
  const auto flags = isActiveLayer ? (_base_node_flags | ImGuiTreeNodeFlags_Selected)  //
                                   : _base_node_flags;

  const auto& context = registry.get<comp::attribute_context>(layerEntity);
  formatted_string name{"{} {}", get_icon(layer.type), context.name};

  if (layer.type != layer_type::group_layer) {
    if (ImGui::Selectable(name.data(), isActiveLayer)) {
      dispatcher.enqueue<select_layer_event>(layer.id);
    }

    /* Make sure to select the layer item when right-clicked as well */
    if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
      dispatcher.enqueue<select_layer_event>(layer.id);
    }

    UpdateLayerItemPopup(registry, dispatcher, layer.id);
  }
  else {
    _show_group_layer_item(registry, dispatcher, layerEntity, layer, flags, name.data());
  }
}

}  // namespace tactile
