#include "layer_dock.hpp"

#include <cassert>  // assert
#include <limits>   // numeric_limits

#include <imgui.h>

#include <tactile-base/tactile_std.hpp>

#include "add_layer_popup.hpp"
#include "layer_item.hpp"
#include "rename_layer_dialog.hpp"
#include "tactile/core/components/layer.hpp"
#include "tactile/core/components/layer_tree_node.hpp"
#include "tactile/core/components/parent.hpp"
#include "tactile/core/components/property_context.hpp"
#include "tactile/core/systems/layers/layer_system.hpp"
#include "tactile/editor/events/layer_events.hpp"
#include "tactile/editor/gui/icons.hpp"
#include "tactile/editor/gui/widgets/alignment.hpp"
#include "tactile/editor/gui/widgets/common/button.hpp"
#include "tactile/editor/gui/widgets/common/centered_text.hpp"
#include "tactile/editor/gui/widgets/common/window.hpp"
#include "tactile/io/preferences.hpp"

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
  auto dock = Window{"Layers", ImGuiWindowFlags_NoCollapse, &isVisible};
  has_focus = dock.IsFocused(ImGuiFocusedFlags_RootAndChildWindows);

  if (dock) {
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

  Prefs::SetShowLayerDock(isVisible);

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
