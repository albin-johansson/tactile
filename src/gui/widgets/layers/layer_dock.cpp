#include "layer_dock.hpp"

#include <imgui.h>

#include <limits>  // numeric_limits

#include "add_layer_popup.hpp"
#include "core/events/layers/add_layer_event.hpp"
#include "core/events/layers/duplicate_layer_event.hpp"
#include "core/events/layers/move_layer_down_event.hpp"
#include "core/events/layers/move_layer_up_event.hpp"
#include "core/events/layers/remove_layer_event.hpp"
#include "core/model.hpp"
#include "gui/icons.hpp"
#include "gui/widgets/common/button_ex.hpp"
#include "io/preferences.hpp"
#include "layer_item.hpp"

namespace Tactile {
namespace {

void UpdateLayerDockButtons(const MapDocument* document,
                            entt::dispatcher& dispatcher)
{
  Maybe<layer_id> activeLayerId;

  if (document)
  {
    activeLayerId = document->GetMap().GetActiveLayerId();
  }

  const bool hasActiveLayer = activeLayerId.has_value();

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
               document && document->CanMoveActiveLayerUp()))
  {
    dispatcher.enqueue<MoveLayerUpEvent>(*activeLayerId);
  }

  ImGui::SameLine();

  if (ButtonEx(TAC_ICON_MOVE_DOWN,
               "Move layer down.",
               document && document->CanMoveActiveLayerDown()))
  {
    dispatcher.enqueue<MoveLayerDownEvent>(*activeLayerId);
  }
}

}  // namespace

void UpdateLayerDock(const Model& model, entt::dispatcher& dispatcher)
{
  if (!Prefs::GetShowLayerDock() || !model.GetActiveMapId())
  {
    return;
  }

  const auto* document = model.GetActiveDocument();
  bool isVisible = Prefs::GetShowLayerDock();

  if (ImGui::Begin("Layers", &isVisible, ImGuiWindowFlags_NoCollapse))
  {
    UpdateLayerDockButtons(document, dispatcher);
    if (document)
    {
      const auto windowHeight = ImGui::GetWindowHeight();
      const auto textLineHeight = ImGui::GetTextLineHeightWithSpacing();
      const auto size = ImVec2{std::numeric_limits<float>::min(),
                               windowHeight - (4 * textLineHeight)};

      if (ImGui::BeginListBox("##LayerTreeNode", size))
      {
        for (const auto& [id, layer] : document->GetMap())
        {
          LayerItem(*document, dispatcher, id, layer);
        }

        ImGui::EndListBox();
      }
    }
  }

  Prefs::SetShowLayerDock(isVisible);
  ImGui::End();
}

}  // namespace Tactile
