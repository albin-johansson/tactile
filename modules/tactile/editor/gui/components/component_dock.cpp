#include "component_dock.hpp"

#include <utility>  // move

#include <imgui.h>
#include <tactile_def.hpp>

#include "core/components/component.hpp"
#include "core/systems/component_system.hpp"
#include "core/systems/property_system.hpp"
#include "editor/events/component_events.hpp"
#include "editor/gui/alignment.hpp"
#include "editor/gui/common/centered_button.hpp"
#include "editor/gui/common/input_widgets.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/scoped.hpp"
#include "io/preferences.hpp"

namespace tactile {
namespace {

constexpr auto gWindowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;

[[nodiscard]] auto TrailingComponentButton() -> bool
{
  ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32_BLACK_TRANS);
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32_BLACK_TRANS);
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32_BLACK_TRANS);

  AlignNextItemToTheRight(TAC_ICON_THREE_DOTS);
  const auto pressed = ImGui::SmallButton(TAC_ICON_THREE_DOTS);

  ImGui::PopStyleColor(3);

  return pressed;
}

void ShowComponentPopupContent(entt::dispatcher& dispatcher,
                               const ContextID contextId,
                               const ComponentID componentId)
{
  if (ImGui::MenuItem(TAC_ICON_RESET " Reset Values")) {
    dispatcher.enqueue<ResetComponentValuesEvent>(contextId, componentId);
  }

  ImGui::Separator();

  if (ImGui::MenuItem(TAC_ICON_COPY " Copy Component")) {
    // TODO
  }

  ImGui::Separator();

  if (ImGui::MenuItem(TAC_ICON_REMOVE " Remove Component")) {
    dispatcher.enqueue<RemoveComponentEvent>(contextId, componentId);
  }
}

void ShowComponent(entt::dispatcher& dispatcher,
                   const ContextID contextId,
                   const CStr name,
                   const comp::component& component)
{
  const scoped::ID componentScope{name};

  constexpr auto headerFlags =
      ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;

  if (scoped::TreeNode node{name, headerFlags}; node.IsOpen()) {
    ImGui::SameLine();
    if (TrailingComponentButton()) {
      ImGui::OpenPopup("##ComponentPopup");
    }

    if (const auto popup = scoped::Popup::ForItem("##ComponentPopup"); popup.IsOpen()) {
      ShowComponentPopupContent(dispatcher, contextId, component.type);
    }

    constexpr auto tableFlags =
        ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable | ImGuiTableFlags_PadOuterX;

    if (scoped::Table table{"##ComponentAttributeTable", 2, tableFlags}; table.IsOpen()) {
      for (const auto& [attributeName, attribute] : component.values) {
        const scoped::ID attributeScope{attributeName.c_str()};

        ImGui::TableNextRow();
        ImGui::TableNextColumn();

        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted(attributeName.c_str());

        ImGui::TableNextColumn();

        if (auto value = Input("##ComponentAttributeTableValue", attribute)) {
          dispatcher.enqueue<UpdateComponentEvent>(contextId,
                                                   component.type,
                                                   attributeName,
                                                   std::move(*value));
        }
      }
    }
  }
}

void ShowAddComponentButtonPopupContent(const entt::registry& registry,
                                        entt::dispatcher& dispatcher,
                                        const ContextID contextId)
{
  const auto view = registry.view<comp::component_def>();
  if (view.empty()) {
    scoped::Disable disable;
    ImGui::TextUnformatted("No available components");
  }
  else {
    for (auto [defEntity, def] : view.each()) {
      scoped::Disable disable{sys::has_component(registry, contextId, def.id)};

      if (ImGui::MenuItem(def.name.c_str())) {
        dispatcher.enqueue<AddComponentEvent>(contextId, def.id);
      }
    }
  }

  ImGui::Separator();
  if (ImGui::MenuItem(TAC_ICON_COMPONENT " Component Editor...")) {
    dispatcher.enqueue<OpenComponentEditorEvent>();
  }
}

}  // namespace

void ComponentDock::Update(const entt::registry& registry, entt::dispatcher& dispatcher)
{
  auto showDock = prefs::GetShowComponentDock();

  if (!showDock) {
    return;
  }

  scoped::Window dock{"Components", gWindowFlags, &showDock};
  mHasFocus = dock.IsFocused();

  if (dock.IsOpen()) {
    const auto& context = sys::GetCurrentContext(registry);
    ImGui::Text("Context: %s", context.name.c_str());

    if (scoped::Child pane{"##ComponentsChild"}; pane.IsOpen()) {
      for (const auto componentEntity : context.components) {
        const auto& component = registry.get<comp::component>(componentEntity);
        const auto& name = sys::get_component_def_name(registry, component.type);

        ImGui::Separator();
        ShowComponent(dispatcher, context.id, name.c_str(), component);
      }

      ImGui::Separator();

      if (CenteredButton(TAC_ICON_ADD, "Add component")) {
        ImGui::OpenPopup("##AddComponentButtonPopup");
      }

      if (scoped::Popup popup{"##AddComponentButtonPopup"}; popup.IsOpen()) {
        ShowAddComponentButtonPopupContent(registry, dispatcher, context.id);
      }
    }
  }

  prefs::SetShowComponentDock(showDock);
}

}  // namespace tactile
