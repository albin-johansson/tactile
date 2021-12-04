#include "layer_dock.hpp"

#include <cassert>  // assert
#include <limits>   // numeric_limits

#include <tactile_def.hpp>

#include <imgui.h>

#include "add_layer_popup.hpp"
#include "core/components/layer.hpp"
#include "core/components/parent.hpp"
#include "core/components/property_context.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "editor/events/layer_events.hpp"
#include "editor/gui/alignment.hpp"
#include "editor/gui/common/button.hpp"
#include "editor/gui/common/centered_text.hpp"
#include "editor/gui/common/window.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/scoped.hpp"
#include "io/preferences.hpp"
#include "layer_item.hpp"
#include "rename_layer_dialog.hpp"

namespace Tactile {
namespace {

constinit bool gShowRenameDialog = false;
constinit bool gHasFocus = false;
inline Maybe<LayerID> gRenameTarget;

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
             activeLayerId && Sys::CanMoveLayerUp(registry, *activeLayerId))) {
    dispatcher.enqueue<MoveLayerUpEvent>(*activeLayerId);
  }

  ImGui::SameLine();

  if (Button(TAC_ICON_MOVE_DOWN,
             "Move layer down",
             activeLayerId && Sys::CanMoveLayerDown(registry, *activeLayerId))) {
    dispatcher.enqueue<MoveLayerDownEvent>(*activeLayerId);
  }
}

}  // namespace

void UpdateLayerDock(const entt::registry& registry,
                     const Icons& icons,
                     entt::dispatcher& dispatcher)
{
  if (!Prefs::GetShowLayerDock()) {
    return;
  }

  bool isVisible = Prefs::GetShowLayerDock();
  auto dock = Window{"Layers", ImGuiWindowFlags_NoCollapse, &isVisible};
  gHasFocus = dock.IsFocused(ImGuiFocusedFlags_RootAndChildWindows);

  if (dock.IsOpen()) {
    UpdateLayerDockButtons(registry, dispatcher);
    if (registry.empty<Layer>()) {
      PrepareVerticalAlignmentCenter(1);
      CenteredText("No available layers!");
    }
    else {
      const auto textLineHeight = ImGui::GetTextLineHeightWithSpacing();
      const auto size = ImVec2{std::numeric_limits<float>::min(),
                               ImGui::GetWindowHeight() - (4 * textLineHeight)};

      if (Scoped::ListBox list{"##LayerTreeNode", size}; list.IsOpen()) {
        for (auto&& [entity, node] : registry.view<LayerTreeNode>().each()) {
          /* Note, we rely on the LayerTreeNode pool being sorted, so we can't include
             other components in the view query directly. */
          const auto& layer = registry.get<Layer>(entity);
          const auto& parent = registry.get<Parent>(entity);
          if (parent.entity == entt::null) {
            LayerItem(registry, icons, dispatcher, entity, layer);
          }
        }
      }
    }
  }

  Prefs::SetShowLayerDock(isVisible);

  if (gShowRenameDialog) {
    const auto target = gRenameTarget.value();

    const auto entity = Sys::FindLayer(registry, target);
    assert(entity != entt::null);

    const auto& context = registry.get<PropertyContext>(entity);

    OpenRenameLayerDialog(gRenameTarget.value(), context.name);
    gRenameTarget.reset();
    gShowRenameDialog = false;
  }

  UpdateRenameLayerDialog(registry, dispatcher);
}

void OpenRenameLayerDialog(const LayerID id)
{
  gRenameTarget = id;
  gShowRenameDialog = true;
}

auto IsLayerDockFocused() noexcept -> bool
{
  return gHasFocus;
}

}  // namespace Tactile