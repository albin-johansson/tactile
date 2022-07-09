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

#include "tooltips.hpp"

#include <chrono>  //

#include <imgui.h>

#include "core/common/associative.hpp"
#include "core/common/maybe.hpp"
#include "misc/assert.hpp"

using namespace std::chrono_literals;

using Clock = std::chrono::system_clock;
using TimePoint = Clock::time_point;

namespace tactile::ui {

void lazy_tooltip(const char* id, const char* tooltip)
{
  TACTILE_ASSERT(id);
  TACTILE_ASSERT(tooltip);

  static HashMap<ImGuiID, Maybe<TimePoint>> state;

  const auto hashedId = ImGui::GetID(id);
  auto&      lastHover = state[hashedId];

  if (ImGui::IsItemHovered()) {
    if (!lastHover) {
      lastHover = Clock::now();
    }

    if (Clock::now() - lastHover.value() > 1s) {
      ImGui::SetTooltip("%s", tooltip);
    }
  }
  else {
    lastHover.reset();
  }
}

}  // namespace tactile::ui