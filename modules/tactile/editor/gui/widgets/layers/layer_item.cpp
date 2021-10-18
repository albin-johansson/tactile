#include "layer_item.hpp"

#include <imgui.h>
#include <rune/core/formatted_string.hpp>  // formatted_string

#include <tactile-base/tactile_std.hpp>

#include "layer_item_popup.hpp"
#include "tactile/core/components/group_layer.hpp"
#include "tactile/core/components/layer_tree_node.hpp"
#include "tactile/core/components/parent.hpp"
#include "tactile/core/components/property_context.hpp"
#include "tactile/core/map.hpp"
#include "tactile/core/utils/scope_id.hpp"
#include "tactile/editor/events/layer_events.hpp"
#include "tactile/editor/gui/icons.hpp"

namespace Tactile {
namespace {

void GroupLayerItem(const entt::registry& registry,
                    entt::dispatcher& dispatcher,
                    const entt::entity layerEntity,
                    const Layer& layer,
                    const ImGuiTreeNodeFlags flags,
                    const CStr name)
{
  ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());
  if (ImGui::TreeNodeEx("##GroupLayerTreeNode", flags, "%s", name)) {
    ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());

    if (ImGui::IsItemActivated() ||
        (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)))
    {
      dispatcher.enqueue<SelectLayerEvent>(layer.id);
    }

    UpdateLayerItemPopup(registry, dispatcher, layer.id);

    const auto& node = registry.get<LayerTreeNode>(layerEntity);
    for (const auto child : node.children) {
      const auto& childLayer = registry.get<Layer>(child);
      LayerItem(registry, dispatcher, child, childLayer);
    }

    ImGui::TreePop();
  }
  else {
    ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());

    if (ImGui::IsItemActivated() ||
        (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)))
    {
      dispatcher.enqueue<SelectLayerEvent>(layer.id);
    }

    UpdateLayerItemPopup(registry, dispatcher, layer.id);
  }
}

}  // namespace

void LayerItem(const entt::registry& registry,
               entt::dispatcher& dispatcher,
               const entt::entity layerEntity,
               const Layer& layer)
{
  const ScopeID uid{layer.id};

  const auto& activeLayer = registry.ctx<ActiveLayer>();
  const auto isActiveLayer = layerEntity == activeLayer.entity;

  auto flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow |
               ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth |
               ImGuiTreeNodeFlags_SpanFullWidth;
  if (isActiveLayer) {
    flags |= ImGuiTreeNodeFlags_Selected;
  }

  const auto& context = registry.get<PropertyContext>(layerEntity);
  rune::formatted_string<128> name{"{} {}", GetIcon(layer.type), context.name};

  if (layer.type != LayerType::GroupLayer) {
    if (ImGui::Selectable(name.data(), isActiveLayer)) {
      dispatcher.enqueue<SelectLayerEvent>(layer.id);
    }

    /* Make sure to select the layer item when right-clicked as well */
    if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
      dispatcher.enqueue<SelectLayerEvent>(layer.id);
    }

    UpdateLayerItemPopup(registry, dispatcher, layer.id);
  }
  else {
    GroupLayerItem(registry, dispatcher, layerEntity, layer, flags, name.data());
  }
}

}  // namespace Tactile
