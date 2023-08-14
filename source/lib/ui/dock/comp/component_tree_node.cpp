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

#include "component_tree_node.hpp"

#include <utility>  // move

#include "model/components/component_components.hpp"
#include "model/entity_validation.hpp"
#include "model/events/component_events.hpp"
#include "tactile/core/common/assert.hpp"
#include "ui/style/alignment.hpp"
#include "ui/style/icons.hpp"
#include "ui/widget/attribute_widgets.hpp"
#include "ui/widget/scoped.hpp"

namespace tactile {
namespace {

constexpr auto kHeaderFlags =
    ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowOverlap;
constexpr auto kTableFlags =
    ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable | ImGuiTableFlags_PadOuterX;

void _push_attribute_table(ModelView model,
                           const Entity attached_component_entity,
                           const AttachedComponent& attached_component)
{
  const auto& strings = model.get_language_strings();

  if (const ui::Table table {"##AttributeTable", 2, kTableFlags}; table.is_open()) {
    for (const auto& [attr_name, attr_value]: attached_component.attributes) {
      const ui::Scope attribute_scope {attr_name.c_str()};

      ImGui::TableNextRow();
      ImGui::TableNextColumn();

      ImGui::AlignTextToFramePadding();
      ImGui::TextUnformatted(attr_name.c_str());

      ImGui::TableNextColumn();
      if (auto value =
              ui::push_attribute_input(strings, "##TableAttribute", attr_value)) {
        model.enqueue<UpdateAttachedComponentEvent>(attached_component_entity,
                                                    attr_name,
                                                    std::move(*value));
      }
    }
  }
}

void _push_trailing_button_popup_content(ModelView model,
                                         const Entity context_entity,
                                         const Entity definition_entity,
                                         const Entity attached_component_entity)
{
  const auto& strings = model.get_language_strings();

  if (ImGui::MenuItem(strings.action.reset_attached_component.c_str())) {
    model.enqueue<ResetAttachedComponentEvent>(attached_component_entity);
  }

  ImGui::Separator();

  if (ImGui::MenuItem(strings.action.detach_component.c_str())) {
    // TODO provide the attached component entity instead of the definition
    model.enqueue<DetachComponentEvent>(context_entity, definition_entity);
  }
}

auto _show_trailing_button() -> bool
{
  const ui::StyleColor button {ImGuiCol_Button, IM_COL32_BLACK_TRANS};
  const ui::StyleColor button_hovered {ImGuiCol_ButtonHovered, IM_COL32_BLACK_TRANS};
  const ui::StyleColor button_active {ImGuiCol_ButtonActive, IM_COL32_BLACK_TRANS};

  ui::right_align_next_item(TAC_ICON_THREE_DOTS);
  const auto pressed = ImGui::SmallButton(TAC_ICON_THREE_DOTS);

  return pressed;
}

}  // namespace

void push_component_tree_node(ModelView model,
                              const Entity context_entity,
                              const Entity attached_component_entity)
{
  const auto& registry = model.get_registry();

  TACTILE_ASSERT(sys::is_context_entity(registry, context_entity));
  TACTILE_ASSERT(sys::is_attached_component_entity(registry, attached_component_entity));

  const auto& attached_component =
      registry.get<AttachedComponent>(attached_component_entity);
  const auto& component = registry.get<Component>(attached_component.definition);

  const ui::Scope component_scope {component.name.c_str()};

  if (const ui::TreeNode header {component.name.c_str(), kHeaderFlags};
      header.is_open()) {
    ImGui::SameLine();
    if (_show_trailing_button()) {
      ImGui::OpenPopup("##ComponentPopup");
    }

    if (auto popup = ui::Popup::for_item("##ComponentPopup"); popup.is_open()) {
      _push_trailing_button_popup_content(model,
                                          context_entity,
                                          attached_component.definition,
                                          attached_component_entity);
    }

    _push_attribute_table(model, attached_component_entity, attached_component);
  }
  else {
    // Show a disabled button when collapsed, to avoid having the button disappear
    const ui::Disable disable;
    ImGui::SameLine();
    _show_trailing_button();
  }
}

}  // namespace tactile