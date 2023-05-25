/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "component_view.hpp"

#include <utility>  // move

#include "core/component.hpp"
#include "model/event/component_events.hpp"
#include "model/systems/language_system.hpp"
#include "ui/style/alignment.hpp"
#include "ui/style/icons.hpp"
#include "ui/widget/attribute_widgets.hpp"
#include "ui/widget/scoped.hpp"

namespace tactile::ui {
namespace {

constexpr auto kHeaderFlags =
    ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;
constexpr auto kTableFlags =
    ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable | ImGuiTableFlags_PadOuterX;

void _push_attribute_table(const Strings& strings,
                           const Entity context_entity,
                           const Entity definition_entity,
                           const Component& component,
                           Dispatcher& dispatcher)
{
  if (Table table {"##AttributeTable", 2, kTableFlags}; table.is_open()) {
    for (const auto& [attr_name, attr_value]: component.attributes) {
      const Scope scope {attr_name.c_str()};

      ImGui::TableNextRow();
      ImGui::TableNextColumn();

      ImGui::AlignTextToFramePadding();
      ImGui::TextUnformatted(attr_name.c_str());

      ImGui::TableNextColumn();
      if (auto value = push_attribute_input(strings, "##TableAttribute", attr_value)) {
        dispatcher.enqueue<UpdateAttachedComponentEvent>(context_entity,
                                                         definition_entity,
                                                         attr_name,
                                                         std::move(*value));
      }
    }
  }
}

void _push_trailing_button_popup_content(const Strings& strings,
                                         const Entity context_entity,
                                         const Entity definition_entity,
                                         Dispatcher& dispatcher)
{
  if (ImGui::MenuItem(strings.action.reset_attached_component.c_str())) {
    dispatcher.enqueue<ResetAttachedComponentEvent>(context_entity, definition_entity);
  }

  ImGui::Separator();

  if (ImGui::MenuItem(strings.action.detach_component.c_str())) {
    dispatcher.enqueue<DetachComponentEvent>(context_entity, definition_entity);
  }
}

auto _show_trailing_button() -> bool
{
  const StyleColor button {ImGuiCol_Button, IM_COL32_BLACK_TRANS};
  const StyleColor button_hovered {ImGuiCol_ButtonHovered, IM_COL32_BLACK_TRANS};
  const StyleColor button_active {ImGuiCol_ButtonActive, IM_COL32_BLACK_TRANS};

  right_align_next_item(TAC_ICON_THREE_DOTS);
  const auto pressed = ImGui::SmallButton(TAC_ICON_THREE_DOTS);

  return pressed;
}

}  // namespace

void component_view(const Model& model,
                    const Entity context_entity,
                    const Entity component_entity,
                    Dispatcher& dispatcher)
{
  const auto& strings = sys::get_current_language_strings(model);
  const auto& component = model.get<Component>(component_entity);
  const auto& definition = model.get<ComponentDefinition>(component.definition);

  const Scope scope {definition.name.c_str()};
  if (TreeNode header {definition.name.c_str(), kHeaderFlags}; header.is_open()) {
    ImGui::SameLine();
    if (_show_trailing_button()) {
      ImGui::OpenPopup("##ComponentPopup");
    }

    if (auto popup = Popup::for_item("##ComponentPopup"); popup.is_open()) {
      _push_trailing_button_popup_content(strings,
                                          context_entity,
                                          component.definition,
                                          dispatcher);
    }

    _push_attribute_table(strings,
                          context_entity,
                          component.definition,
                          component,
                          dispatcher);
  }
  else {
    // Show a disabled button when collapsed, to avoid having the button disappear
    Disable disable;
    ImGui::SameLine();
    _show_trailing_button();
  }
}

}  // namespace tactile::ui