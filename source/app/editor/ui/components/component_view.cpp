/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
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

#include <entt/signal/dispatcher.hpp>

#include "core/components/component.hpp"
#include "core/events/component_events.hpp"
#include "editor/ui/alignment.hpp"
#include "editor/ui/common/input_widgets.hpp"
#include "editor/ui/icons.hpp"
#include "editor/ui/scoped.hpp"

namespace tactile::ui {
namespace {

constexpr auto _header_flags =
    ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;
constexpr auto _table_flags =
    ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable | ImGuiTableFlags_PadOuterX;

void _update_attribute_table(const UUID&       contextId,
                             const Component&  component,
                             entt::dispatcher& dispatcher)
{
  if (Table table{"##AttributeTable", 2, _table_flags}; table.is_open()) {
    for (const auto& [attrName, attrValue] : component) {
      const Scope scope{attrName.c_str()};

      ImGui::TableNextRow();
      ImGui::TableNextColumn();

      ImGui::AlignTextToFramePadding();
      ImGui::TextUnformatted(attrName.c_str());

      ImGui::TableNextColumn();
      if (auto value = input_attribute("##ComponentAttributeTableValue", attrValue)) {
        dispatcher.enqueue<UpdateAttachedComponentEvent>(contextId,
                                                         component.definition_id(),
                                                         attrName,
                                                         std::move(*value));
      }
    }
  }
}

void _update_trailing_button_popup_content(const UUID&       contextId,
                                           const Component&  component,
                                           entt::dispatcher& dispatcher)
{
  if (ImGui::MenuItem(TAC_ICON_RESET " Reset Values")) {
    dispatcher.enqueue<ResetAttachedComponentEvent>(contextId, component.definition_id());
  }

  ImGui::Separator();

  {
    Disable disable;
    if (ImGui::MenuItem(TAC_ICON_COPY " Copy Component")) {
      // TODO
    }
  }

  ImGui::Separator();

  if (ImGui::MenuItem(TAC_ICON_REMOVE " Remove Component")) {
    dispatcher.enqueue<DetachComponentEvent>(contextId, component.definition_id());
  }
}

auto _update_trailing_button() -> bool
{
  StyleColor button{ImGuiCol_Button, IM_COL32_BLACK_TRANS};
  StyleColor buttonHovered{ImGuiCol_ButtonHovered, IM_COL32_BLACK_TRANS};
  StyleColor buttonActive{ImGuiCol_ButtonActive, IM_COL32_BLACK_TRANS};

  right_align_next_item(TAC_ICON_THREE_DOTS);
  const auto pressed = ImGui::SmallButton(TAC_ICON_THREE_DOTS);

  return pressed;
}

}  // namespace

void component_view(const UUID&        contextId,
                    const Component&   component,
                    const std::string& name,
                    entt::dispatcher&  dispatcher)
{
  const Scope scope{name.c_str()};

  if (ImGui::CollapsingHeader(name.c_str(), _header_flags)) {
    ImGui::SameLine();
    if (_update_trailing_button()) {
      ImGui::OpenPopup("##ComponentPopup");
    }

    if (auto popup = Popup::for_item("##ComponentPopup"); popup.is_open()) {
      _update_trailing_button_popup_content(contextId, component, dispatcher);
    }

    _update_attribute_table(contextId, component, dispatcher);
  }
  else {
    // Show a disabled button when collapsed, to avoid having the button disappear
    Disable disable;
    ImGui::SameLine();
    _update_trailing_button();
  }
}

}  // namespace tactile::ui