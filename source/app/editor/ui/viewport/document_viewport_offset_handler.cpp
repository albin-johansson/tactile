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

#include "document_viewport_offset_handler.hpp"

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>

#include "core/events/viewport_events.hpp"

namespace tactile::ui {
namespace {

constexpr auto _button_flags = ImGuiButtonFlags_MouseButtonLeft |
                               ImGuiButtonFlags_MouseButtonMiddle |
                               ImGuiButtonFlags_MouseButtonRight;

}  // namespace

void update_document_viewport_offset(const ImVec2&     viewportSize,
                                     entt::dispatcher& dispatcher)
{
  ImGui::InvisibleButton("update_document_viewport_offset", viewportSize, _button_flags);
  if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Middle)) {
    const auto&    io = ImGui::GetIO();
    const Vector2f delta {io.MouseDelta.x, io.MouseDelta.y};
    dispatcher.enqueue<OffsetDocumentViewportEvent>(delta);
  }
}

}  // namespace tactile::ui
