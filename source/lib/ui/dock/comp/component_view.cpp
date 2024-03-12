// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "component_view.hpp"

#include <utility>  // move

#include <entt/signal/dispatcher.hpp>

#include "core/component/component.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/event/component_events.hpp"
#include "ui/style/alignment.hpp"
#include "ui/style/icons.hpp"
#include "ui/widget/attribute_widgets.hpp"
#include "ui/widget/scoped.hpp"

namespace tactile::ui {
namespace {

constexpr auto kHeaderFlags =
    ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowOverlap;
constexpr auto kTableFlags =
    ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable | ImGuiTableFlags_PadOuterX;

void update_attribute_table(const UUID& context_id,
                            const Component& component,
                            entt::dispatcher& dispatcher)
{
  if (Table table {"##AttributeTable", 2, kTableFlags}; table.is_open()) {
    for (const auto& [attr_name, attr_value]: component) {
      const Scope scope {attr_name.c_str()};

      ImGui::TableNextRow();
      ImGui::TableNextColumn();

      ImGui::AlignTextToFramePadding();
      ImGui::TextUnformatted(attr_name.c_str());

      ImGui::TableNextColumn();
      if (auto value = ui_attribute_input("##TableAttribute", attr_value)) {
        dispatcher.enqueue<UpdateAttachedComponentEvent>(context_id,
                                                         component.definition_id(),
                                                         attr_name,
                                                         std::move(*value));
      }
    }
  }
}

void update_trailing_button_popup_content(const UUID& context_id,
                                          const Component& component,
                                          entt::dispatcher& dispatcher)
{
  const auto& lang = get_current_language();

  if (ImGui::MenuItem(lang.action.reset_attached_component.c_str())) {
    dispatcher.enqueue<ResetAttachedComponentEvent>(context_id,
                                                    component.definition_id());
  }

  ImGui::Separator();

  if (ImGui::MenuItem(lang.action.detach_component.c_str())) {
    dispatcher.enqueue<DetachComponentEvent>(context_id, component.definition_id());
  }
}

auto update_trailing_button() -> bool
{
  const StyleColor button {ImGuiCol_Button, IM_COL32_BLACK_TRANS};
  const StyleColor button_hovered {ImGuiCol_ButtonHovered, IM_COL32_BLACK_TRANS};
  const StyleColor button_active {ImGuiCol_ButtonActive, IM_COL32_BLACK_TRANS};

  right_align_next_item(TAC_ICON_THREE_DOTS);
  const auto pressed = ImGui::SmallButton(TAC_ICON_THREE_DOTS);

  return pressed;
}

}  // namespace

void component_view(const UUID& context_id,
                    const Component& component,
                    const String& name,
                    entt::dispatcher& dispatcher)
{
  const Scope scope {name.c_str()};
  if (TreeNode header {name.c_str(), kHeaderFlags}; header.is_open()) {
    ImGui::SameLine();
    if (update_trailing_button()) {
      ImGui::OpenPopup("##ComponentPopup");
    }

    if (auto popup = Popup::for_item("##ComponentPopup"); popup.is_open()) {
      update_trailing_button_popup_content(context_id, component, dispatcher);
    }

    update_attribute_table(context_id, component, dispatcher);
  }
  else {
    // Show a disabled button when collapsed, to avoid having the button disappear
    Disable disable;
    ImGui::SameLine();
    update_trailing_button();
  }
}

}  // namespace tactile::ui
