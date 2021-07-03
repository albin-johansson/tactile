#include "layer_widget.hpp"

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
#include "core/events/set_layer_opacity_event.hpp"
#include "core/events/show_layer_properties_event.hpp"
#include "core/model.hpp"
#include "gui/icons.hpp"
#include "gui/widgets/button_ex.hpp"
#include "io/preferences.hpp"
#include "utils/scope_id.hpp"

namespace Tactile {
namespace {

void UpdateLayerItemPopup(const MapDocument& document,
                          entt::dispatcher& dispatcher,
                          const layer_id id)
{
  if (ImGui::BeginPopupContextItem())
  {
    if (ImGui::MenuItem(TAC_ICON_PROPERTIES " Show properties"))
    {
      dispatcher.enqueue<ShowLayerPropertiesEvent>(id);
    }

    ImGui::Separator();
    if (ImGui::MenuItem(TAC_ICON_VISIBILITY " Toggle visibility",
                        nullptr,
                        true))
    {
      // TODO dispatcher.enqueue<ToggleLayerVisibilityEvent>(id);
    }

    auto opacity = document.GetMap().GetOpacity(id);
    ImGui::AlignTextToFramePadding();
    ImGui::Text(TAC_ICON_OPACITY " Opacity");
    ImGui::SameLine();
    if (ImGui::SliderFloat("##OpacitySlider", &opacity, 0, 1.0f))
    {
      dispatcher.enqueue<SetLayerOpacityEvent>(id, opacity);
    }

    ImGui::Separator();
    if (ImGui::MenuItem(TAC_ICON_MOVE_UP " Move layer up",
                        nullptr,
                        false,
                        document.CanMoveLayerUp(id)))
    {
      dispatcher.enqueue<MoveLayerUpEvent>(id);
    }

    if (ImGui::MenuItem(TAC_ICON_MOVE_DOWN " Move layer down",
                        nullptr,
                        false,
                        document.CanMoveLayerDown(id)))
    {
      dispatcher.enqueue<MoveLayerDownEvent>(id);
    }

    ImGui::Separator();
    if (ImGui::MenuItem(TAC_ICON_DUPLICATE " Duplicate layer"))
    {
      dispatcher.enqueue<DuplicateLayerEvent>(id);
    }

    if (ImGui::MenuItem(TAC_ICON_REMOVE " Remove layer"))
    {
      dispatcher.enqueue<RemoveLayerEvent>(id);
    }

    ImGui::EndPopup();
  }
}

void UpdateLayers(const MapDocument& document, entt::dispatcher& dispatcher)
{
  const auto& map = document.GetMap();
  const auto activeLayerId = map.GetActiveLayerId();
  for (const auto& [id, layer] : map)
  {
    const ScopeID uid{id};

    const auto icon = layer->GetType() == LayerType::ObjectLayer
                          ? TAC_ICON_OBJECT_LAYER
                          : TAC_ICON_TILE_LAYER;
    const auto name = std::format("{} {}", icon, layer->GetName());

    const auto isSelected = id == activeLayerId;
    ImGui::Selectable(name.c_str(), isSelected);

    if (ImGui::IsItemActivated() ||
        (ImGui::IsItemHovered() &&
         ImGui::IsMouseClicked(ImGuiMouseButton_Right)))
    {
      dispatcher.enqueue<SelectLayerEvent>(id);
    }

    UpdateLayerItemPopup(document, dispatcher, id);

    if (isSelected)
    {
      ImGui::SetItemDefaultFocus();
    }
  }
}

void UpdateAddLayerPopup(entt::dispatcher& dispatcher)
{
  if (ImGui::BeginPopup("AddLayerPopup"))
  {
    if (ImGui::Selectable(TAC_ICON_TILE_LAYER " Tile layer"))
    {
      dispatcher.enqueue<AddLayerEvent>(LayerType::TileLayer);
    }

    if (ImGui::Selectable(TAC_ICON_OBJECT_LAYER " Object layer"))
    {
      dispatcher.enqueue<AddLayerEvent>(LayerType::ObjectLayer);
    }

    ImGui::EndPopup();
  }
}

}  // namespace

void UpdateLayerWidget(const Model& model, entt::dispatcher& dispatcher)
{
  if (!Prefs::GetShowLayerDock() || !model.GetActiveMapId())
  {
    return;
  }

  const auto* document = model.GetActiveDocument();
  const auto& map = document->GetMap();
  const auto hasActiveLayer = map.GetActiveLayerId().has_value();
  const auto activeLayerId = map.GetActiveLayerId();

  bool isVisible = Prefs::GetShowLayerDock();
  if (ImGui::Begin("Layers", &isVisible, ImGuiWindowFlags_NoCollapse))
  {
    if (ButtonEx(TAC_ICON_ADD, "Add new layer."))
    {
      ImGui::OpenPopup("AddLayerPopup");
    }

    UpdateAddLayerPopup(dispatcher);

    ImGui::SameLine();
    if (ButtonEx(TAC_ICON_REMOVE, "Remove layer.", hasActiveLayer))
    {
      dispatcher.enqueue<RemoveLayerEvent>(*activeLayerId);
    }

    ImGui::SameLine();
    if (ButtonEx(TAC_ICON_DUPLICATE, "Duplicate layer.", hasActiveLayer))
    {
      dispatcher.enqueue<DuplicateLayerEvent>(*activeLayerId);
    }

    ImGui::SameLine();

    if (ButtonEx(TAC_ICON_MOVE_UP,
                 "Move layer up.",
                 document->CanMoveActiveLayerUp()))
    {
      dispatcher.enqueue<MoveLayerUpEvent>(*activeLayerId);
    }

    ImGui::SameLine();

    if (ButtonEx(TAC_ICON_MOVE_DOWN,
                 "Move layer down.",
                 document->CanMoveActiveLayerDown()))
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

  Prefs::SetShowLayerDock(isVisible);
  ImGui::End();
}

}  // namespace Tactile
