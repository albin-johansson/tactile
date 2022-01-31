#include "layer_dock.hpp"

#define NOMINMAX

#include <limits>  // numeric_limits

#include <imgui.h>

#include "add_layer_popup.hpp"
#include "assert.hpp"
#include "core/components/attribute_context.hpp"
#include "core/components/layer.hpp"
#include "core/components/parent.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/layer_tree_system.hpp"
#include "editor/events/layer_events.hpp"
#include "editor/gui/alignment.hpp"
#include "editor/gui/common/button.hpp"
#include "editor/gui/common/centered_text.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/scoped.hpp"
#include "editor/model.hpp"
#include "io/preferences.hpp"
#include "layer_item.hpp"
#include "tactile_def.hpp"

namespace tactile {
namespace {

void UpdateLayerDockButtons(const entt::registry& registry, entt::dispatcher& dispatcher)
{
  const auto activeLayerEntity = registry.ctx<comp::active_layer>().entity;
  const auto hasActiveLayer = activeLayerEntity != entt::null;

  maybe<layer_id> activeLayerId;
  if (hasActiveLayer) {
    const auto& layer = registry.get<comp::layer>(activeLayerEntity);
    activeLayerId = layer.id;
  }

  scoped::Group group;

  if (Button(TAC_ICON_ADD, "Add new layer")) {
    OpenAddLayerPopup();
  }

  UpdateAddLayerPopup(dispatcher);

  if (Button(TAC_ICON_REMOVE, "Remove layer", hasActiveLayer)) {
    dispatcher.enqueue<RemoveLayerEvent>(*activeLayerId);
  }

  if (Button(TAC_ICON_DUPLICATE, "Duplicate layer", hasActiveLayer)) {
    dispatcher.enqueue<DuplicateLayerEvent>(*activeLayerId);
  }

  if (Button(TAC_ICON_MOVE_UP,
             "Move layer up",
             hasActiveLayer && sys::can_move_layer_up(registry, activeLayerEntity))) {
    dispatcher.enqueue<MoveLayerUpEvent>(*activeLayerId);
  }

  if (Button(TAC_ICON_MOVE_DOWN,
             "Move layer down",
             hasActiveLayer && sys::can_move_layer_down(registry, activeLayerEntity))) {
    dispatcher.enqueue<MoveLayerDownEvent>(*activeLayerId);
  }
}

}  // namespace

void LayerDock::Update(const Model& model,
                       const Icons& icons,
                       entt::dispatcher& dispatcher)
{
  auto& prefs = get_preferences();
  bool visible = prefs.is_layer_dock_visible();

  if (!visible) {
    return;
  }

  scoped::Window dock{"Layers", ImGuiWindowFlags_NoCollapse, &visible};
  mHasFocus = dock.IsFocused(ImGuiFocusedFlags_RootAndChildWindows);

  const auto& registry = model.GetActiveRegistryRef();

  if (dock.IsOpen()) {
    UpdateLayerDockButtons(registry, dispatcher);
    ImGui::SameLine();

    scoped::Group group;
    if (registry.view<comp::layer>().empty()) {
      PrepareVerticalAlignmentCenter(1);
      CenteredText("No available layers!");
    }
    else {
      const ImVec2 size{-(std::numeric_limits<float>::min)(),
                        -(std::numeric_limits<float>::min)()};
      if (scoped::ListBox list{"##LayerTreeNode", size}; list.IsOpen()) {
        for (auto&& [entity, node] : registry.view<comp::layer_tree_node>().each()) {
          /* Note, we rely on the LayerTreeNode pool being sorted, so we can't include
             other components in the view query directly. */
          const auto& layer = registry.get<comp::layer>(entity);
          const auto& parent = registry.get<comp::parent>(entity);
          if (parent.entity == entt::null) {
            LayerItem(registry, icons, dispatcher, entity, layer);
          }
        }
      }
    }
  }

  prefs.set_layer_dock_visible(visible);

  if (mRenameTarget.has_value()) {
    const auto target = *mRenameTarget;

    const auto entity = sys::find_layer(registry, target);
    TACTILE_ASSERT(entity != entt::null);

    const auto& context = registry.get<comp::attribute_context>(entity);

    mRenameLayerDialog.Show(target, context.name);
    mRenameTarget.reset();
  }

  mRenameLayerDialog.Update(model, dispatcher);
}

void LayerDock::ShowRenameLayerDialog(const layer_id id)
{
  mRenameTarget = id;
}

}  // namespace tactile
