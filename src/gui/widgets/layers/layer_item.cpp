#include "layer_item.hpp"

#include <imgui.h>

#include <format>  // format

#include "aliases/czstring.hpp"
#include "core/events/layers/select_layer_event.hpp"
#include "core/map/layers/layer_utils.hpp"
#include "core/map_document.hpp"
#include "icons.hpp"
#include "layer_item_popup.hpp"
#include "utils/scope_id.hpp"

namespace Tactile {
namespace {

void GroupLayerItem(const MapDocument& document,
                    entt::dispatcher& dispatcher,
                    const layer_id id,
                    const SharedLayer& layer,
                    const ImGuiTreeNodeFlags flags,
                    const czstring name)
{
  ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());
  if (ImGui::TreeNodeEx("##GroupLayerTreeNode", flags, "%s", name))
  {
    ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());

    if (ImGui::IsItemActivated() ||
        (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)))
    {
      dispatcher.enqueue<SelectLayerEvent>(id);
    }

    UpdateLayerItemPopup(document, dispatcher, id);

    if (const auto* group = AsGroupLayer(layer))
    {
      for (const auto& [subid, sublayer] : *group)
      {
        LayerItem(document, dispatcher, subid, sublayer);
      }
    }

    ImGui::TreePop();
  }
  else
  {
    ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
  }
}

}  // namespace

void LayerItem(const MapDocument& document,
               entt::dispatcher& dispatcher,
               const layer_id id,
               const SharedLayer& layer)
{
  const auto& map = document.GetMap();
  const auto activeLayer = map.GetActiveLayerId();

  const ScopeID uid{id};

  auto flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow |
               ImGuiTreeNodeFlags_OpenOnDoubleClick |
               ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth;
  if (id == activeLayer)
  {
    flags |= ImGuiTreeNodeFlags_Selected;
  }

  const auto name =
      std::format("{} {}", GetIcon(layer->GetType()), layer->GetName());

  if (layer->GetType() == LayerType::GroupLayer)
  {
    GroupLayerItem(document, dispatcher, id, layer, flags, name.c_str());
  }
  else
  {
    if (ImGui::Selectable(name.c_str(), id == activeLayer))
    {
      dispatcher.enqueue<SelectLayerEvent>(id);
    }

    UpdateLayerItemPopup(document, dispatcher, id);
  }
}

}  // namespace Tactile
