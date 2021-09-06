#include "layer_item.hpp"

#include <imgui.h>

#include <format>  // format

#include "aliases/cstr.hpp"
#include "core/components/group_layer.hpp"
#include "core/components/parent.hpp"
#include "core/components/property_context.hpp"
#include "core/map.hpp"
#include "events/layer_events.hpp"
#include "gui/icons.hpp"
#include "layer_item_popup.hpp"
#include "utils/scope_id.hpp"

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
  if (ImGui::TreeNodeEx("##GroupLayerTreeNode", flags, "%s", name))
  {
    ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());

    if (ImGui::IsItemActivated() ||
        (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)))
    {
      dispatcher.enqueue<SelectLayerEvent>(layer.id);
    }

    UpdateLayerItemPopup(registry, dispatcher, layer.id);

    const auto& groupLayer = registry.get<GroupLayer>(layerEntity);
    for (const auto child : groupLayer.layers)
    {
      const auto& childLayer = registry.get<Layer>(child);
      LayerItem(registry, dispatcher, child, childLayer);
    }

    ImGui::TreePop();
  }
  else
  {
    ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
  }
}

}  // namespace

void LayerItem(const entt::registry& registry,
               entt::dispatcher& dispatcher,
               const entt::entity layerEntity,
               const Layer& layer)
{
  const ScopeID uid{layer.id};

  const auto& map = registry.ctx<Map>();
  const auto& activeLayer = registry.ctx<ActiveLayer>();
  const auto isActiveLayer = layerEntity == activeLayer.entity;

  auto flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow |
               ImGuiTreeNodeFlags_OpenOnDoubleClick |
               ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth;
  if (isActiveLayer)
  {
    flags |= ImGuiTreeNodeFlags_Selected;
  }

  const auto& context = registry.get<PropertyContext>(layerEntity);
  const auto name = std::format("{} {}", GetIcon(layer.type), context.name);

  if (layer.type != LayerType::GroupLayer)
  {
    if (ImGui::Selectable(name.c_str(), isActiveLayer))
    {
      dispatcher.enqueue<SelectLayerEvent>(layer.id);
    }

    UpdateLayerItemPopup(registry, dispatcher, layer.id);
  }
  else
  {
    GroupLayerItem(registry, dispatcher, layerEntity, layer, flags, name.c_str());
  }
}

}  // namespace Tactile
