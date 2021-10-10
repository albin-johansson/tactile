#include "layer_dock.hpp"

#include <imgui.h>

#include <cassert>  // assert
#include <limits>   // numeric_limits

#include "add_layer_popup.hpp"
#include "common/maybe.hpp"
#include "core/components/layer.hpp"
#include "core/components/layer_tree_node.hpp"
#include "core/components/parent.hpp"
#include "core/components/property_context.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "events/layer_events.hpp"
#include "gui/icons.hpp"
#include "gui/widgets/alignment.hpp"
#include "gui/widgets/common/button.hpp"
#include "gui/widgets/common/centered_text.hpp"
#include "io/preferences.hpp"
#include "layer_item.hpp"
#include "rename_layer_dialog.hpp"

namespace Tactile {
namespace {

constinit bool show_rename_dialog = false;
constinit bool has_focus = false;
inline Maybe<LayerID> rename_target;

void UpdateLayerDockButtons(const entt::registry& registry, entt::dispatcher& dispatcher)
{
  const auto activeLayerEntity = registry.ctx<ActiveLayer>().entity;
  const auto hasActiveLayer = activeLayerEntity != entt::null;

  Maybe<LayerID> activeLayerId;
  if (hasActiveLayer) {
    const auto& layer = registry.get<Layer>(activeLayerEntity);
    activeLayerId = layer.id;
  }

  if (Button(TAC_ICON_ADD, "Add new layer")) {
    OpenAddLayerPopup();
  }

  UpdateAddLayerPopup(dispatcher);

  ImGui::SameLine();
  if (Button(TAC_ICON_REMOVE, "Remove layer", hasActiveLayer)) {
    dispatcher.enqueue<RemoveLayerEvent>(*activeLayerId);
  }

  ImGui::SameLine();
  if (Button(TAC_ICON_DUPLICATE, "Duplicate layer", hasActiveLayer)) {
    dispatcher.enqueue<DuplicateLayerEvent>(*activeLayerId);
  }

  ImGui::SameLine();

  if (Button(TAC_ICON_MOVE_UP,
             "Move layer up",
             activeLayerId && Sys::CanMoveLayerUp(registry, *activeLayerId)))
  {
    dispatcher.enqueue<MoveLayerUpEvent>(*activeLayerId);
  }

  ImGui::SameLine();

  if (Button(TAC_ICON_MOVE_DOWN,
             "Move layer down",
             activeLayerId && Sys::CanMoveLayerDown(registry, *activeLayerId)))
  {
    dispatcher.enqueue<MoveLayerDownEvent>(*activeLayerId);
  }
}

}  // namespace

void UpdateLayerDock(const entt::registry& registry, entt::dispatcher& dispatcher)
{
  if (!Prefs::GetShowLayerDock()) {
    return;
  }

  bool isVisible = Prefs::GetShowLayerDock();
  if (ImGui::Begin("Layers", &isVisible, ImGuiWindowFlags_NoCollapse)) {
    has_focus = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);

    UpdateLayerDockButtons(registry, dispatcher);
    if (registry.empty<Layer>()) {
      PrepareVerticalAlignmentCenter(1);
      CenteredText("No available layers!");
    }
    else {
      const auto textLineHeight = ImGui::GetTextLineHeightWithSpacing();
      const auto size = ImVec2{std::numeric_limits<float>::min(),
                               ImGui::GetWindowHeight() - (4 * textLineHeight)};

      if (ImGui::BeginListBox("##LayerTreeNode", size)) {
        for (auto&& [entity, node] : registry.view<LayerTreeNode>().each()) {
          /* Note, we rely on the LayerTreeNode pool being sorted, so we can't include
             other components in the view query directly. */
          const auto& layer = registry.get<Layer>(entity);
          const auto& parent = registry.get<Parent>(entity);
          if (parent.entity == entt::null) {
            LayerItem(registry, dispatcher, entity, layer);
          }
        }

        ImGui::EndListBox();
      }
    }
  }
  else {
    has_focus = false;
  }

  Prefs::SetShowLayerDock(isVisible);
  ImGui::End();

  if (show_rename_dialog) {
    const auto target = rename_target.value();

    const auto entity = Sys::FindLayer(registry, target);
    assert(entity != entt::null);

    const auto& context = registry.get<PropertyContext>(entity);

    OpenRenameLayerDialog(rename_target.value(), context.name);
    rename_target.reset();
    show_rename_dialog = false;
  }

  UpdateRenameLayerDialog(registry, dispatcher);
}

void OpenRenameLayerDialog(const LayerID id)
{
  rename_target = id;
  show_rename_dialog = true;
}

auto IsLayerDockFocused() noexcept -> bool
{
  return has_focus;
}

}  // namespace Tactile
