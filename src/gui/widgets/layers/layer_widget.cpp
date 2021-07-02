#include "layer_widget.hpp"

#include <IconsFontAwesome5.h>
#include <imgui.h>

#include <format>  // format
#include <limits>  // numeric_limits

#include "aliases/czstring.hpp"
#include "core/events/add_layer_event.hpp"
#include "core/events/duplicate_layer_event.hpp"
#include "core/events/move_layer_down_event.hpp"
#include "core/events/move_layer_up_event.hpp"
#include "core/events/remove_layer_event.hpp"
#include "core/events/select_layer_event.hpp"
#include "core/model.hpp"
#include "gui/widgets/button_ex.hpp"
#include "utils/scope_id.hpp"

namespace Tactile {
namespace {

inline bool is_visible = true;

void UpdateLayers(const MapDocument& document, entt::dispatcher& dispatcher)
{
  const auto& map = document.GetMap();
  const auto activeLayerId = map.GetActiveLayerId();
  for (const auto& [id, layer] : map)
  {
    const ScopeID uid{id};

    const auto icon = layer->GetType() == LayerType::ObjectLayer
                          ? ICON_FA_SHAPES
                          : ICON_FA_LAYER_GROUP;
    const auto name = std::format("{} {}", icon, layer->GetName());

    const auto isSelected = id == activeLayerId;
    ImGui::Selectable(name.c_str(), isSelected);

    if (ImGui::IsItemActivated())
    {
      dispatcher.enqueue<SelectLayerEvent>(id);
    }

    if (isSelected)
    {
      ImGui::SetItemDefaultFocus();
    }
  }
}

}  // namespace

void UpdateLayerWidget(const Model& model, entt::dispatcher& dispatcher)
{
  if (!is_visible || !model.GetActiveMapId())
  {
    return;
  }

  const auto* document = model.GetActiveDocument();
  const auto& map = document->GetMap();
  const auto hasActiveLayer = map.GetActiveLayerId().has_value();
  const auto activeLayerId = map.GetActiveLayerId();

  if (ImGui::Begin("Layers", &is_visible, ImGuiWindowFlags_NoCollapse))
  {
    if (ButtonEx(ICON_FA_PLUS_CIRCLE, "Add new layer."))
    {
      ImGui::OpenPopup("AddLayerPopup");
    }

    if (ImGui::BeginPopup("AddLayerPopup"))
    {
      if (ImGui::Selectable(ICON_FA_LAYER_GROUP " Tile layer"))
      {
        dispatcher.enqueue<AddLayerEvent>(LayerType::TileLayer);
      }

      if (ImGui::Selectable(ICON_FA_SHAPES " Object layer"))
      {
        dispatcher.enqueue<AddLayerEvent>(LayerType::ObjectLayer);
      }

      ImGui::EndPopup();
    }

    ImGui::SameLine();
    if (ButtonEx(ICON_FA_MINUS_CIRCLE, "Remove layer.", hasActiveLayer))
    {
      dispatcher.enqueue<RemoveLayerEvent>(*activeLayerId);
    }

    ImGui::SameLine();
    if (ButtonEx(ICON_FA_COPY, "Duplicate layer.", hasActiveLayer))
    {
      dispatcher.enqueue<DuplicateLayerEvent>(*activeLayerId);
    }

    ImGui::SameLine();

    const auto canMoveUp = hasActiveLayer && map.IndexOf(*activeLayerId) != 0;
    if (ButtonEx(ICON_FA_ARROW_UP, "Move layer up.", canMoveUp))
    {
      dispatcher.enqueue<MoveLayerUpEvent>(*activeLayerId);
    }

    ImGui::SameLine();

    const auto canMoveDown = hasActiveLayer && (map.IndexOf(*activeLayerId) !=
                                                map.GetLayerCount() - 1);
    if (ButtonEx(ICON_FA_ARROW_DOWN, "Move layer down.", canMoveDown))
    {
      dispatcher.enqueue<MoveLayerDownEvent>(*activeLayerId);
    }

    const auto windowHeight = ImGui::GetWindowHeight();
    const auto textLineHeight = ImGui::GetTextLineHeightWithSpacing();
    const auto size = ImVec2{std::numeric_limits<float>::min(),
                             windowHeight - (4 * textLineHeight)};

    if (ImGui::BeginListBox("##LayerTreeNode", size))
    {
      if (document)
      {
        UpdateLayers(*document, dispatcher);
      }

      ImGui::EndListBox();
    }
  }

  ImGui::End();
}

void SetLayerWidgetVisible(const bool visible) noexcept
{
  is_visible = visible;
}

auto IsLayerWidgetVisible() noexcept -> bool
{
  return is_visible;
}

}  // namespace Tactile
